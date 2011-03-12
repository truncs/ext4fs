/*-
 * Copyright (c) 2010, 2010 Zheng Liu <lz@freebsd.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $FreeBSD: src/sys/fs/ext4fs/ext4_extents.c,v 0.1 2010/09/16 20:37:00 lz Exp $
 */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/types.h>
#include <sys/kernel.h>
#include <sys/malloc.h>
#include <sys/vnode.h>
#include <sys/bio.h>
#include <sys/buf.h>
#include <sys/conf.h>

#include <fs/ext4fs/ext4_mount.h>
#include <fs/ext4fs/fs.h>
#include <fs/ext4fs/inode.h>
#include <fs/ext4fs/ext4fs.h>
#include <fs/ext4fs/ext4_extents.h>
#include <fs/ext4fs/ext4_extern.h>

static void ext4_ext_binsearch_index(struct inode *, struct ext4_extent_path *, daddr_t);
static void ext4_ext_binsearch(struct inode *, struct ext4_extent_path *, daddr_t);

static void
ext4_ext_binsearch_index(struct inode *ip, struct ext4_extent_path *path, daddr_t lbn)
{
        struct ext4_extent_header *ehp = path->ep_header;
        struct ext4_extent_index *l, *r, *m;

        l = (struct ext4_extent_index *)(((char *)(ehp) +
            sizeof(struct ext4_extent_header)));
        r = (struct ext4_extent_index *)(((char *)(ehp) +
            sizeof(struct ext4_extent_header))) + ehp->eh_ecount - 1;
        while (l <= r) {
                m = l + (r - l) / 2;
                if (lbn < m->ei_blk)
                        r = m - 1;
                else
                        l = m + 1;
        }

        path->ep_index = l - 1;
}

static void
ext4_ext_binsearch(struct inode *ip, struct ext4_extent_path *path, daddr_t lbn)
{
        struct ext4_extent_header *ehp = path->ep_header;
        struct ext4_extent *l, *r, *m;

        if (ehp->eh_ecount == 0)
                return;

        l = (struct ext4_extent *)(((char *)(ehp) +
            sizeof(struct ext4_extent_header)));
        r = (struct ext4_extent *)(((char *)(ehp) +
            sizeof(struct ext4_extent_header))) + ehp->eh_ecount - 1;
        while (l <= r) {
                m = l + (r - l) / 2;
                if (lbn < m->e_blk)
                        r = m - 1;
                else
                        l = m + 1;
        }

        path->ep_ext = l - 1;
}

/*
 * find a block in ext4 extent cache.
 */
int
ext4_ext_in_cache(struct inode *ip, daddr_t lbn, struct ext4_extent *ep)
{
        struct ext4_extent_cache *ecp;
        int ret = EXT4_EXT_CACHE_NO;

        ecp = &ip->i_ext_cache;

        /* cache is invalid */
        if (ecp->ec_type == EXT4_EXT_CACHE_NO)
                return (ret);

        if (lbn >= ecp->ec_blk && lbn < ecp->ec_blk + ecp->ec_len) {
                ep->e_blk = ecp->ec_blk;
                ep->e_start_lo = (ecp->ec_start & 0xffffffff);
                ep->e_start_hi = (((ecp->ec_start >> 31) >> 1) & 0xffff);
                ep->e_len = ecp->ec_len;
                ret = ecp->ec_type;
        }

        return (ret);
}

/*
 * put a ext4_extent structure in ext4 cache.
 */
void
ext4_ext_put_cache(struct inode *ip, struct ext4_extent *ep, int type)
{
        struct ext4_extent_cache *ecp;

        ecp = &ip->i_ext_cache;
        ecp->ec_type = type;
        ecp->ec_blk = ep->e_blk;
        ecp->ec_len = ep->e_len;
        ecp->ec_start = (((daddr_t)(ep->e_start_hi) << 31) << 1) | ep->e_start_lo;
}

/*
 * find a extent.
 */
struct ext4_extent_path *
ext4_ext_find_extent(struct m_ext2fs *fs, struct inode *ip,
    daddr_t lbn, struct ext4_extent_path *path)
{
        struct vnode *vp;
        struct ext4_extent_header *ehp;
        int depth, i, error, size;
        daddr_t nblk;

        vp = ITOV(ip);
        ehp = (struct ext4_extent_header *)((char *)ip->i_db);
        depth = ehp->eh_depth;

        if (ehp->eh_magic != EXT4_EXT_MAGIC)
                return (NULL);

        path->ep_header = ehp;

        i = depth;
        while (i) {
                ext4_ext_binsearch_index(ip, path, lbn);
                path->ep_blk = (((daddr_t)(path->ep_index->ei_leaf_hi) << 31) << 1) |
                    path->ep_index->ei_leaf_lo;
                path->ep_depth = i;
                path->ep_ext = NULL;

                size = blksize(fs, ip, path->ep_blk);
                nblk = path->ep_blk;
                if (path->ep_bp != NULL) {
                        brelse(path->ep_bp);
                        path->ep_bp = NULL;
                }
                error = bread(ip->i_devvp, fsbtodb(fs, nblk), size, NOCRED, &path->ep_bp);
                if (error) {
                        brelse(path->ep_bp);
                        path->ep_bp = NULL;
                        return (NULL);
                }
                ehp = (struct ext4_extent_header *)path->ep_bp->b_data;
                path->ep_header = ehp;
                i--;
        }

        path->ep_depth = i;
        path->ep_ext = NULL;
        path->ep_index = NULL;

        ext4_ext_binsearch(ip, path, lbn);
        if (path->ep_ext != NULL)
                path->ep_blk = (((daddr_t)(path->ep_ext->e_start_hi) << 31) << 1) |
                    path->ep_ext->e_start_lo;

        return (path);
}
