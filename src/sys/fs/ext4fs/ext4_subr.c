/*-
 *  modified for Lites 1.1
 *
 *  Aug 1995, Godmar Back (gback@cs.utah.edu)
 *  University of Utah, Department of Computer Science
 */
/*-
 * Copyright (c) 1982, 1986, 1989, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)ffs_subr.c	8.2 (Berkeley) 9/21/93
 * $FreeBSD: src/sys/fs/ext4fs/ext4_subr.c,v 1.1 2010/09/16 20:44:00 lz Exp $
 */

#include <sys/param.h>

#include <sys/proc.h>
#include <sys/systm.h>
#include <sys/bio.h>
#include <sys/buf.h>
#include <sys/lock.h>
#include <sys/ucred.h>
#include <sys/vnode.h>

#include <fs/ext4fs/inode.h>
#include <fs/ext4fs/ext4_extern.h>
#include <fs/ext4fs/ext4fs.h>
#include <fs/ext4fs/fs.h>
#include <fs/ext4fs/ext4_extents.h>

#ifdef KDB
void	ext2_checkoverlap(struct buf *, struct inode *);
#endif

/*
 * Return buffer with the contents of block "offset" from the beginning of
 * directory "ip".  If "res" is non-zero, fill it in with a pointer to the
 * remaining space in the directory.
 */
int
ext2_blkatoff(vp, offset, res, bpp)
	struct vnode *vp;
	off_t offset;
	char **res;
	struct buf **bpp;
{
	struct inode *ip;
	struct m_ext2fs *fs;
	struct buf *bp;
        struct ext4_extent *ep;
        struct ext4_extent_header *ehp;
        struct ext4_extent_path path;
	int32_t lbn;
	int bsize, error;
        int depth;
        daddr_t newblk;

	ip = VTOI(vp);
	fs = ip->i_e2fs;
	lbn = lblkno(fs, offset);
	bsize = blksize(fs, ip, lbn);
        memset(&path, 0, sizeof(path));

	*bpp = NULL;
        if (ext4_ext_find_extent(fs, ip, lbn, &path) == NULL)
                goto normal;
        depth = ((struct ext4_extent_header *)(ip->i_db))->eh_depth;
        if (path.ep_ext == NULL && depth != 0)
                goto normal;
        ehp = path.ep_header;
        ep = path.ep_ext;
        if (ep == NULL)
                goto normal;

        newblk = lbn - ep->e_blk +
            (ep->e_start_lo | ((daddr_t)(ep->e_start_hi) << 31) << 1);

        if (path.ep_bp != NULL) {
                brelse(path.ep_bp);
                path.ep_bp = NULL;
        }
        if ((error = bread(ip->i_devvp, fsbtodb(fs, newblk), bsize, NOCRED, &bp)) != 0) {
                brelse(bp);
                return (error);
        }
        if (res)
                *res = (char *)bp->b_data + blkoff(fs, offset);
        *bpp = bp;
        return (0);

normal:
        if (*bpp == NULL) {
                if ((error = bread(vp, lbn, bsize, NOCRED, &bp)) != 0) {
                        brelse(bp);
                        return (error);
                }
                if (res)
                        *res = (char *)bp->b_data + blkoff(fs, offset);
                *bpp = bp;
        }
	return (0);
}

#ifdef KDB
void
ext2_checkoverlap(bp, ip)
	struct buf *bp;
	struct inode *ip;
{
	struct buf *ebp, *ep;
	int32_t start, last;
	struct vnode *vp;

	ebp = &buf[nbuf];
	start = bp->b_blkno;
	last = start + btodb(bp->b_bcount) - 1;
	for (ep = buf; ep < ebp; ep++) {
		if (ep == bp || (ep->b_flags & B_INVAL))
			continue;
		vp = ip->i_devvp;
		/* look for overlap */
		if (ep->b_bcount == 0 || ep->b_blkno > last ||
		    ep->b_blkno + btodb(ep->b_bcount) <= start)
			continue;
		vprint("Disk overlap", vp);
		(void)printf("\tstart %d, end %d overlap start %lld, end %ld\n",
			start, last, (long long)ep->b_blkno,
			(long)(ep->b_blkno + btodb(ep->b_bcount) - 1));
		panic("Disk buffer overlap");
	}
}
#endif /* KDB */
