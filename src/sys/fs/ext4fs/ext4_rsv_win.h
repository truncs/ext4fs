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
 * $FreeBSD: src/sys/fs/ext4fs/ext4_rsv_win.h,v 0.1 2010/09/16 20:43:51 lz Exp $
 */
#ifndef _FS_EXT4FS_EXT4_RSV_WIN_H_
#define _FS_EXT4FS_EXT4_RSV_WIN_H_

#include <sys/tree.h>

#define EXT2_RSV_DEFAULT_RESERVE_BLKS 8
#define EXT2_RSV_MAX_RESERVE_BLKS 1024
#define EXT2_RSV_NOT_ALLOCATED 0

#define EXT2_RSV_LOCK(ip)   mtx_lock(&ip->i_rsv_lock)
#define EXT2_RSV_UNLOCK(ip) mtx_unlock(&ip->i_rsv_lock)

#define EXT2_TREE_LOCK(fs)   mtx_lock(&fs->e2fs_rsv_lock);
#define EXT2_TREE_UNLOCK(fs) mtx_unlock(&fs->e2fs_rsv_lock);

/*
 * Reservation window entry
 */
struct ext2_rsv_win {
	RB_ENTRY(ext2_rsv_win) rsv_link; /* RB tree links */

	int32_t rsv_goal_size; /* Default reservation window size */
	int32_t rsv_alloc_hit; /* Number of allocated windows */

	int32_t rsv_start; /* First bytes of window */
	int32_t rsv_end;   /* End bytes of window */
};

RB_HEAD(ext2_rsv_win_tree, ext2_rsv_win);

static __inline int
ext2_rsv_win_cmp(const struct ext2_rsv_win *a,
		 const struct ext2_rsv_win *b)
{
	if (a->rsv_start < b->rsv_start)
		return -1;
	if (a->rsv_start == b->rsv_start)
		return 0;

	return 1;
}
RB_PROTOTYPE(ext2_rsv_win_tree, ext2_rsv_win, rsv_link, ext2_rsv_win_cmp);

struct inode;
struct ucred;
/* ext2_alloc.c */
void    ext2_init_rsv(struct inode *ip);
void    ext2_discard_rsv(struct inode *ip);
int     ext2_alloc_rsv(struct inode *, int32_t, int32_t,
                       int, struct ucred *, int32_t *);

#endif /* !_FS_EXT4FS_EXT4_RSV_WIN_H_ */
