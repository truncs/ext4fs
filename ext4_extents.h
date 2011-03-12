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
 * $FreeBSD: src/sys/fs/ext4fs/ext4_extents.h,v 0.1 2010/09/16 20:38:51 lz Exp $
 */
#ifndef _FS_EXT4FS_EXT4_EXTENTS_H_
#define _FS_EXT4FS_EXT4_EXTENTS_H_

#include <sys/types.h>

#define EXT4_EXT_MAGIC  0xf30a

/* lock/unlock ext lock */
#define EXT4_EXT_LOCK(ip)       mtx_lock(&(ip)->i_ext_lock)
#define EXT4_EXT_UNLOCK(ip)     mtx_unlock(&(ip)->i_ext_lock)

#define EXT4_EXT_CACHE_NO       0
#define EXT4_EXT_CACHE_GAP      1
#define EXT4_EXT_CACHE_IN       2

/*
 * ext4 file system extent on disk
 */
struct ext4_extent {
        u_int32_t e_blk;        /* first logical block */
        u_int16_t e_len;        /* number of blocks */
        u_int16_t e_start_hi;   /* high 16 bits of physical block */
        u_int32_t e_start_lo;   /* low 32 bits of physical block */
};

/*
 * extent index on disk
 */
struct ext4_extent_index {
        u_int32_t ei_blk;       /* indexes logical blocks */
        u_int32_t ei_leaf_lo;   /* pointes to physical block of the next level */
        u_int16_t ei_leaf_hi;   /* high 16 bits of physical block */
        u_int16_t ei_unused;
};

/*
 * extent tree header
 */
struct ext4_extent_header {
        u_int16_t eh_magic;     /* magic number: 0xf30a */
        u_int16_t eh_ecount;    /* number of valid entries */
        u_int16_t eh_max;       /* capacity of store in entries */
        u_int16_t eh_depth;     /* the depth of extent tree */
        u_int32_t eh_gen;       /* generation of extent tree */
};

/*
 * save cached extent
 */
struct ext4_extent_cache {
        daddr_t   ec_start;     /* extent start */
        u_int32_t ec_blk;       /* logical block */
        u_int32_t ec_len;
        u_int32_t ec_type;
};

/*
 * save path to some extent.
 */
struct ext4_extent_path {
        daddr_t   ep_blk;
        u_int16_t ep_depth;
        struct    buf *ep_bp;
        struct ext4_extent        *ep_ext;
        struct ext4_extent_index  *ep_index;
        struct ext4_extent_header *ep_header;
};

struct inode;
struct m_ext2fs;
int     ext4_ext_in_cache(struct inode *, daddr_t, struct ext4_extent *);
void    ext4_ext_put_cache(struct inode *, struct ext4_extent *, int);
struct  ext4_extent_path *ext4_ext_find_extent(struct m_ext2fs *fs, struct inode *,
                                               daddr_t, struct ext4_extent_path *);

#endif /* !_FS_EXT4FS_EXT4_EXTENTS_H_ */
