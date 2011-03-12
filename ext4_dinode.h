/*-
 * Copyright (c) 2009 Aditya Sarawgi
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
 * $FreeBSD: src/sys/fs/ext4fs/ext4_dinode.h,v 1.1 2010/09/16 20:34:00 lz Exp $
 */

#ifndef _FS_EXT4FS_EXT4_DINODE_H_
#define _FS_EXT4FS_EXT4_DINODE_H_

/*
 * Inode flags
 * The current implementation uses only EXT2_IMMUTABLE and EXT2_APPEND flags
 */
#define EXT2_SECRM		0x00000001	/* Secure deletion */
#define EXT2_UNRM		0x00000002	/* Undelete */
#define EXT2_COMPR		0x00000004	/* Compress file */
#define EXT2_SYNC		0x00000008	/* Synchronous updates */
#define EXT2_IMMUTABLE		0x00000010	/* Immutable file */
#define EXT2_APPEND		0x00000020	/* writes to file may only append */
#define EXT2_NODUMP		0x00000040	/* do not dump file */
#define EXT2_NOATIME		0x00000080	/* do not update atime */
/* NOT implementation. Reserved for compression usage. */
#define EXT4_DIRTY              0x00000100
#define EXT4_COMPRBLK           0x00000200      /* One or more compressed clusters */
#define EXT4_NOCOMPR            0x00000400      /* Don't compress */
#define EXT4_ECOMPR             0x00000800      /* Compression error */ 
/* End compression flags */
#define EXT4_INDEX              0x00001000      /* Hash-indexed directory */
#define EXT4_IMAGIC             0x00002000      /* AFS directory */
#define EXT4_JOURNAL_DATA       0x00004000      /* File data should be journaled */
#define EXT4_NOTAIL             0x00008000      /* File tail should not be merged */
#define EXT4_DIRSYNC            0x00010000      /* dirsync behavior */
#define EXT4_TOPDIR             0x00020000      /* top of directory hierarchies */
#define EXT4_HUGE_FILE          0x00040000      /* Set to each huge file */
#define EXT4_EXTENTS            0x00080000      /* Inode uses extents */
#define EXT4_RESERVED           0x80000000      /* Reserved for ext4 lib */

/*
 * Structure of an inode on the disk
 */
struct ext2fs_dinode {
	u_int16_t	e2di_mode;	/*   0: IFMT, permissions; see below. */
	u_int16_t	e2di_uid;	/*   2: Owner UID */
	u_int32_t	e2di_size_lo;	/*	 4: Size (in bytes) */
	u_int32_t	e2di_atime;	/*	 8: Access time */
	u_int32_t	e2di_ctime;	/*	12: Create time */
	u_int32_t	e2di_mtime;	/*	16: Modification time */
	u_int32_t	e2di_dtime;	/*	20: Deletion time */
	u_int16_t	e2di_gid;	/*  24: Owner GID */
	u_int16_t	e2di_nlink;	/*  26: File link count */
	u_int32_t	e2di_nblock_lo;	/*  28: Blocks count */
	u_int32_t	e2di_flags;	/*  32: Status flags (chflags) */
        union {
                struct {
                        u_int32_t e2di_version;
                } linux1;
                struct {
                        u_int32_t e2di_translator;
                } hurd1;
                struct {
                        u_int32_t e2di_reserved1;
                } masix1;
        } osd1;                         /* 36: */
	u_int32_t	e2di_blocks[EXT2_N_BLOCKS]; /* 40: disk blocks */
	u_int32_t	e2di_gen;	/* 100: generation number */
	u_int32_t	e2di_facl;	/* 104: file ACL (not implemented) */
	u_int32_t	e2di_dacl;	/* 108: dir ACL (not implemented) */
#define e2di_size_high	e2di_dacl
	u_int32_t	e2di_faddr;	/* 112: fragment address */
        union {
                struct {
                        u_int16_t e2di_l_blk_high;
#define e2di_nblock_high        osd2.linux2.e2di_l_blk_high
                        u_int16_t e2di_l_facl_high;
                        u_int16_t e2di_l_uid_high;
                        u_int16_t e2di_l_gid_high;
                        u_int32_t e2di_l_reserved2;
                } linux2;
                struct {
                        u_int16_t e2di_h_reserved1;
                        u_int16_t e2di_h_mode_high;
                        u_int16_t e2di_h_uid_high;
                        u_int16_t e2di_h_gid_high;
                        u_int32_t e2di_h_author;
                } hurd2;
                struct {
                        u_int16_t e2di_m_reserved1;
                        u_int16_t e2di_m_facl_high;
                        u_int32_t e2di_reserved2[2];
                } masix2;
        } osd2;
        u_int16_t e2di_extra_isize;
        u_int16_t e2di_pad;
        u_int32_t e2di_ctime_extra;
        u_int32_t e2di_mtime_extra;
        u_int32_t e2di_atime_extra;
        u_int32_t e2di_crtime;
        u_int32_t e2di_crtime_extra;
        u_int32_t e2di_version_hi;
};

#endif /* _FS_EXT4FS_EXT4_DINODE_H_ */
