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
 * $FreeBSD: src/sys/fs/ext4fs/ext4_htree.h,v 0.1 2010/09/16 20:40:00 lz Exp $
 */
#ifndef _FS_EXT4FS_EXT4_HTREE_H_
#define _FS_EXT4FS_EXT4_HTREE_H_

#define DIRINDEX_HASH_LEGACY            0
#define DIRINDEX_HASH_HALF_MD4          1
#define DIRINDEX_HASH_TEA               2
#define DIRINDEX_HASH_LEGACY_UNSIGNED   3
#define DIRINDEX_HASH_HALF_MD4_UNSIGNED 4
#define DIRINDEX_HASH_TEA_UNSIGNED      5

#define DIRINDEX_BAD_DIR -75000

/*
 * define some data structures for hash directory index.
 */
struct fake_dirent {
        u_int32_t ino;
        u_int16_t reclen;
        u_int8_t  namlen;
        u_int8_t  filetype;
};

struct dirindex_countlimit {
        u_int16_t di_limit;
        u_int16_t di_count;
};

struct dirindex_entry {
        u_int32_t di_hash;
        u_int32_t di_blk;
};

struct dirindex_root {
        struct fake_dirent di_dot;
        char di_dotname[4];
        struct fake_dirent di_dotdot;
        char di_dotdotname[4];
        struct dirindex_root_info {
                u_int32_t reserved;
                u_int8_t hash_version;
                u_int8_t info_len;
                u_int8_t ind_levels;
                u_int8_t unused;
        } di_info;
};

struct dirindex_node {
        struct fake_dirent di_fake;
        struct dirindex_entry di_ent[4];
};

struct dirindex_frame {
        struct dirindex_entry di_ent;
        struct dirindex_entry di_at;
};

struct dirindex_map_entry {
        u_int32_t di_hash;
        u_int16_t di_offset;
        u_int16_t di_size;
};

struct dirindex_hash_info {
        u_int32_t di_hash;
        u_int32_t di_minhash;
        int       di_hashversion;
        u_int32_t *di_seed;
};

/* ext4_htree.c */
int ext4_is_dirindex(struct inode *);
int ext4_dirindex_lookup(struct vnode *, char *, int,
                         doff_t *, struct buf **, doff_t *prevoffp);

#endif /* !_FS_EXT4FS_EXT4_HTREE_H_ */
