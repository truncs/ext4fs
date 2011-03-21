/*-
 *  modified for Lites 1.1
 *
 *  Aug 1995, Godmar Back (gback@cs.utah.edu)
 *  University of Utah, Department of Computer Science
 */
/*-
 * Copyright (c) 1993
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
 *	@(#)ufs_readwrite.c	8.7 (Berkeley) 1/21/94
 * $FreeBSD: src/sys/fs/ext4fs/ext4_readwrite.c,v 1.1 2010/09/16 20:43:00 lz Exp $
 */

#include <fs/ext4fs/ext4_dinode.h>
#include <fs/ext4fs/ext4_extents.h>

/* XXX TODO: remove these obfuscations (as in ffs_vnops.c). */
#define	BLKSIZE(a, b, c)	blksize(a, b, c)
#define	FS			struct m_ext2fs
#define	I_FS			i_e2fs
#define	READ			ext4_read
#define	READ_S			"ext2_read"
#define	WRITE			ext4_write
#define	WRITE_S			"ext2_write"

static int ext4_ext_read(struct vop_read_args *);
static int ext2_ind_read(struct vop_read_args *);

/*
 * this function handles ext4 extents block mapping
 */
static int
ext4_ext_read(struct vop_read_args *ap)
{
	struct vnode *vp;
	struct inode *ip;
        struct uio *uio;
	struct m_ext2fs *fs;
        struct buf *bp;
        struct ext4_extent nex, *ep;
        struct ext4_extent_header *ehp;
        struct ext4_extent_path path;
        daddr_t lbn, nextlbn, newblk = 0;
        off_t bytesinfile;
        u_short mode;
        int cache_type;
	int orig_resid;
        int error = 0;
        int depth = 0;
        long size, xfersize, blkoffset;

	vp = ap->a_vp;
	ip = VTOI(vp);
        mode = ip->i_mode;
        uio = ap->a_uio;
        memset(&path, 0, sizeof(path));

	orig_resid = uio->uio_resid;
	KASSERT(orig_resid >= 0, ("ext2_read: uio->uio_resid < 0"));
	if (orig_resid == 0)
		return (0);
	KASSERT(uio->uio_offset >= 0, ("ext2_read: uio->uio_offset < 0"));
	fs = ip->I_FS;
	if (uio->uio_offset < ip->i_size && uio->uio_offset >= fs->e2fs_maxfilesize)
		return (EOVERFLOW);

        for (error = 0, bp = NULL; uio->uio_resid > 0; bp = NULL) {
                if ((bytesinfile = ip->i_size - uio->uio_offset) <= 0)
                        break;
                lbn = lblkno(fs, uio->uio_offset);
                nextlbn = lbn + 1;
                size = BLKSIZE(fs, ip, lbn);
                blkoffset = blkoff(fs, uio->uio_offset);

                xfersize = fs->e2fs_fsize - blkoffset;
                if (uio->uio_resid < xfersize)
                        xfersize = uio->uio_resid;
                if (bytesinfile < xfersize)
                        xfersize = bytesinfile;

                /* get block from ext4 extent cache */
                cache_type = ext4_ext_in_cache(ip, lbn, &nex);
                if (cache_type != 0) {
                        /* block does not be allocated yet */
                        if (cache_type == EXT4_EXT_CACHE_GAP)
                                return (error);
                        else if (cache_type == EXT4_EXT_CACHE_IN)
                                newblk = lbn - nex.e_blk +
                                    (nex.e_start_lo | ((daddr_t)(nex.e_start_hi) << 31) << 1);
                } else {
                        ext4_ext_find_extent(fs, ip, lbn, &path);
                        depth = ((struct ext4_extent_header *)(ip->i_db))->eh_depth;
                        if (path.ep_ext == NULL && depth != 0)
                                return (EIO);

                        ehp = path.ep_header;
                        ep = path.ep_ext;
                        if (ep == NULL)
                                return (EIO);

                        ext4_ext_put_cache(ip, ep, EXT4_EXT_CACHE_IN);

                        newblk = lbn - ep->e_blk +
                            (ep->e_start_lo | ((daddr_t)(ep->e_start_hi) << 31) << 1);

                        if (path.ep_bp != NULL) {
                                brelse(path.ep_bp);
                                path.ep_bp = NULL;
                        }
                }

                error = bread(ip->i_devvp, fsbtodb(fs, newblk), size, NOCRED, &bp);
                if (error) {
                        brelse(bp);
                        bp = NULL;
                        break;
                }

                size -= bp->b_resid;
                if (size < xfersize) {
                        if (size == 0)
                                break;
                        xfersize = size;
                }
                error = uiomove((char *)bp->b_data + blkoffset,
                    (int)xfersize, uio);
                if (error)
                        break;

                bqrelse(bp);
        }

        if (bp != NULL)
                bqrelse(bp);

        return (error);
}

/*
 * this function handles traditional block mapping
 */
static int
ext2_ind_read(struct vop_read_args *ap)
{
	struct vnode *vp;
	struct inode *ip;
	struct uio *uio;
	FS *fs;
	struct buf *bp;
	daddr_t lbn, nextlbn;
	off_t bytesinfile;
	long size, xfersize, blkoffset;
	int error, orig_resid, seqcount;
	seqcount = ap->a_ioflag >> IO_SEQSHIFT;
	u_short mode;

	vp = ap->a_vp;
	ip = VTOI(vp);
	mode = ip->i_mode;
	uio = ap->a_uio;

#ifdef DIAGNOSTIC
	if (uio->uio_rw != UIO_READ)
		panic("%s: mode", READ_S);

	if (vp->v_type == VLNK) {
		if ((int)ip->i_size < vp->v_mount->mnt_maxsymlinklen)
			panic("%s: short symlink", READ_S);
	} else if (vp->v_type != VREG && vp->v_type != VDIR)
		panic("%s: type %d", READ_S, vp->v_type);
#endif
	orig_resid = uio->uio_resid;
	KASSERT(orig_resid >= 0, ("ext2_read: uio->uio_resid < 0"));
	if (orig_resid == 0)
		return (0);
	KASSERT(uio->uio_offset >= 0, ("ext2_read: uio->uio_offset < 0"));
	fs = ip->I_FS;
	if (uio->uio_offset < ip->i_size && uio->uio_offset >= fs->e2fs_maxfilesize)
		return (EOVERFLOW);
	for (error = 0, bp = NULL; uio->uio_resid > 0; bp = NULL) {
		if ((bytesinfile = ip->i_size - uio->uio_offset) <= 0)
			break;
		lbn = lblkno(fs, uio->uio_offset);
		nextlbn = lbn + 1;
		size = BLKSIZE(fs, ip, lbn);
		blkoffset = blkoff(fs, uio->uio_offset);

		xfersize = fs->e2fs_fsize - blkoffset;
		if (uio->uio_resid < xfersize)
			xfersize = uio->uio_resid;
		if (bytesinfile < xfersize)
			xfersize = bytesinfile;

		if (lblktosize(fs, nextlbn) >= ip->i_size)
			error = bread(vp, lbn, size, NOCRED, &bp);
		else if ((vp->v_mount->mnt_flag & MNT_NOCLUSTERR) == 0)
		error = cluster_read(vp, ip->i_size, lbn, size,
  			NOCRED, blkoffset + uio->uio_resid, seqcount, &bp);
		else if (seqcount > 1) {
			int nextsize = BLKSIZE(fs, ip, nextlbn);
			error = breadn(vp, lbn,
			    size, &nextlbn, &nextsize, 1, NOCRED, &bp);
		} else
			error = bread(vp, lbn, size, NOCRED, &bp);
		if (error) {
			brelse(bp);
			bp = NULL;
			break;
		}

		/*
		 * We should only get non-zero b_resid when an I/O error
		 * has occurred, which should cause us to break above.
		 * However, if the short read did not cause an error,
		 * then we want to ensure that we do not uiomove bad
		 * or uninitialized data.
		 */
		size -= bp->b_resid;
		if (size < xfersize) {
			if (size == 0)
				break;
			xfersize = size;
		}
		error = uiomove((char *)bp->b_data + blkoffset,
  			(int)xfersize, uio);
		if (error)
			break;

		bqrelse(bp);
	}
	if (bp != NULL)
		bqrelse(bp);
	if ((error == 0 || uio->uio_resid != orig_resid) &&
	    (vp->v_mount->mnt_flag & MNT_NOATIME) == 0)
		ip->i_flag |= IN_ACCESS;
	return (error);
}

/*
 * Vnode op for reading.
 */
static int
READ(ap)
	struct vop_read_args /* {
		struct vnode *a_vp;
		struct uio *a_uio;
		int a_ioflag;
		struct ucred *a_cred;
	} */ *ap;
{
	struct vnode *vp;
	struct inode *ip;
        int error;

	vp = ap->a_vp;
	ip = VTOI(vp);

        /*EXT4_EXT_LOCK(ip);*/
        if (ip->i_flags & EXT4_EXTENTS)
                error = ext4_ext_read(ap);
        else
                error = ext2_ind_read(ap);
        /*EXT4_EXT_UNLOCK(ip);*/

        return (error);
}

/*
 * Vnode op for writing.
 */
static int
WRITE(ap)
	struct vop_write_args /* {
		struct vnode *a_vp;
		struct uio *a_uio;
		int a_ioflag;
		struct ucred *a_cred;
	} */ *ap;
{
	struct vnode *vp;
	struct uio *uio;
	struct inode *ip;
	FS *fs;
	struct buf *bp;
	struct thread *td;
	daddr_t lbn;
	off_t osize;
	int blkoffset, error, flags, ioflag, resid, size, seqcount, xfersize;

	ioflag = ap->a_ioflag;
	seqcount = ioflag >> IO_SEQSHIFT;
	uio = ap->a_uio;
	vp = ap->a_vp;
	ip = VTOI(vp);

#ifdef DIAGNOSTIC
	if (uio->uio_rw != UIO_WRITE)
		panic("%s: mode", WRITE_S);
#endif

	switch (vp->v_type) {
	case VREG:
		if (ioflag & IO_APPEND)
			uio->uio_offset = ip->i_size;
		if ((ip->i_flags & APPEND) && uio->uio_offset != ip->i_size)
			return (EPERM);
		/* FALLTHROUGH */
	case VLNK:
		break;
	case VDIR:
		/* XXX differs from ffs -- this is called from ext2_mkdir(). */
		if ((ioflag & IO_SYNC) == 0)
		panic("ext2_write: nonsync dir write");
		break;
	default:
		panic("ext2_write: type %p %d (%jd,%jd)", (void *)vp,
		    vp->v_type, (intmax_t)uio->uio_offset,
		    (intmax_t)uio->uio_resid);
	}

	KASSERT(uio->uio_resid >= 0, ("ext2_write: uio->uio_resid < 0"));
	KASSERT(uio->uio_offset >= 0, ("ext2_write: uio->uio_offset < 0"));
	fs = ip->I_FS;
	if ((uoff_t)uio->uio_offset + uio->uio_resid > fs->e2fs_maxfilesize)
		return (EFBIG);
	/*
	 * Maybe this should be above the vnode op call, but so long as
	 * file servers have no limits, I don't think it matters.
	 */
	td = uio->uio_td;
	if (vp->v_type == VREG && td != NULL) {
		PROC_LOCK(td->td_proc);
		if (uio->uio_offset + uio->uio_resid >
		    lim_cur(td->td_proc, RLIMIT_FSIZE)) {
			psignal(td->td_proc, SIGXFSZ);
			PROC_UNLOCK(td->td_proc);
			return (EFBIG);
		}
		PROC_UNLOCK(td->td_proc);
	}

	resid = uio->uio_resid;
	osize = ip->i_size;
	flags = ioflag & IO_SYNC ? B_SYNC : 0;

	for (error = 0; uio->uio_resid > 0;) {
		lbn = lblkno(fs, uio->uio_offset);
		blkoffset = blkoff(fs, uio->uio_offset);
		xfersize = fs->e2fs_fsize - blkoffset;
		if (uio->uio_resid < xfersize)
			xfersize = uio->uio_resid;
		if (uio->uio_offset + xfersize > ip->i_size)
			vnode_pager_setsize(vp, uio->uio_offset + xfersize);

		/*
		 * Avoid a data-consistency race between write() and mmap()
		 * by ensuring that newly allocated blocks are zeroed. The
		 * race can occur even in the case where the write covers
		 * the entire block.
		 */
		flags |= B_CLRBUF;
		error = ext2_balloc(ip, lbn, blkoffset + xfersize,
			ap->a_cred, &bp, flags);
		if (error != 0)
			break;
		if (uio->uio_offset + xfersize > ip->i_size)
			ip->i_size = uio->uio_offset + xfersize;
		size = BLKSIZE(fs, ip, lbn) - bp->b_resid;
		if (size < xfersize)
			xfersize = size;

		error =
		    uiomove((char *)bp->b_data + blkoffset, (int)xfersize, uio);
		if ((ioflag & IO_VMIO) &&
		   LIST_FIRST(&bp->b_dep) == NULL) /* in ext2fs? */
			bp->b_flags |= B_RELBUF;

		if (ioflag & IO_SYNC) {
			(void)bwrite(bp);
		} else if (xfersize + blkoffset == fs->e2fs_fsize) {
			if ((vp->v_mount->mnt_flag & MNT_NOCLUSTERW) == 0) {
				bp->b_flags |= B_CLUSTEROK;
				cluster_write(vp, bp, ip->i_size, seqcount);
			} else {
				bawrite(bp);
			}
		} else {
			bp->b_flags |= B_CLUSTEROK;
			bdwrite(bp);
		}
		if (error || xfersize == 0)
			break;
	}
	/*
	 * If we successfully wrote any data, and we are not the superuser
	 * we clear the setuid and setgid bits as a precaution against
	 * tampering.
	 * XXX too late, the tamperer may have opened the file while we
	 * were writing the data (or before).
	 * XXX too early, if (error && ioflag & IO_UNIT) then we will
	 * unwrite the data.
	 */
	if (resid > uio->uio_resid && ap->a_cred && ap->a_cred->cr_uid != 0)
		ip->i_mode &= ~(ISUID | ISGID);
	if (error) {
                /*
                 * XXX should truncate to the last successfully written
                 * data if the uiomove() failed.
                 */
		if (ioflag & IO_UNIT) {
			(void)ext2_truncate(vp, osize,
			    ioflag & IO_SYNC, ap->a_cred, uio->uio_td);
			uio->uio_offset -= resid - uio->uio_resid;
			uio->uio_resid = resid;
		}
	}
	if (uio->uio_resid != resid) {
               ip->i_flag |= IN_CHANGE | IN_UPDATE;
               if (ioflag & IO_SYNC)
                       error = ext2_update(vp, 1);
       }
	return (error);
}
