/*-
 *  modified for EXT2FS support in Lites 1.1
 *
 *  Aug 1995, Godmar Back (gback@cs.utah.edu)
 *  University of Utah, Department of Computer Science
 *
 * $FreeBSD: src/sys/fs/ext4fs/ext4fs.h,v 1.1 2010/09/16 20:46:00 lz Exp $
 */
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
 * 
 */

#ifndef _FS_EXT4FS_EXT4_FS_H
#define _FS_EXT4FS_EXT4_FS_H

#include <sys/types.h>
#include <sys/lock.h>

#include <fs/ext4fs/ext4_rsv_win.h>

/*
 * Special inode numbers
 */
#define	EXT2_BAD_INO		 1	/* Bad blocks inode */
#define EXT2_ROOT_INO		 2	/* Root inode */
#define EXT2_BOOT_LOADER_INO	 5	/* Boot loader inode */
#define EXT2_UNDEL_DIR_INO	 6	/* Undelete directory inode */

/* First non-reserved inode for old ext2 filesystems */
#define E2FS_REV0_FIRST_INO	11

/*
 * The second extended file system magic number
 */
#define E2FS_MAGIC		0xEF53

#if defined(_KERNEL)
/*
 * FreeBSD passes the pointer to the in-core struct with relevant
 * fields to EXT2_SB macro when accessing superblock fields.
 */
#define EXT2_SB(sb)	(sb)
#else
/* Assume that user mode programs are passing in an ext2fs superblock, not
 * a kernel struct super_block.  This will allow us to call the feature-test
 * macros from user land. */
#define EXT2_SB(sb)	(sb)
#endif

/*
 * Maximal count of links to a file
 */
#define EXT2_LINK_MAX		65000

/*
 * Constants relative to the data blocks
 */
#define	EXT2_NDIR_BLOCKS		12
#define	EXT2_IND_BLOCK			EXT2_NDIR_BLOCKS
#define	EXT2_DIND_BLOCK			(EXT2_IND_BLOCK + 1)
#define	EXT2_TIND_BLOCK			(EXT2_DIND_BLOCK + 1)
#define	EXT2_N_BLOCKS			(EXT2_TIND_BLOCK + 1)
#define EXT2_MAXSYMLINKLEN		(EXT2_N_BLOCKS * sizeof (uint32_t))

/*
 * The path name on which the file system is mounted is maintained
 * in fs_fsmnt. MAXMNTLEN defines the amount of space allocated in
 * the super block for this name.
 */
#define MAXMNTLEN 512

/* ext4 flex block group data structure */
struct ext4_flex_groups {
        long e2fg_nifree;
        long e2fg_nbfree;
        long e2fg_ndirs;
};

#define EXT4_FLEX_ALLOC_DIR_SIZE 4

/*
 * Super block for an ext2fs file system.
 */
struct ext2fs {
	u_int32_t  e2fs_icount;		/* Inode count */
	u_int32_t  e2fs_bcount_lo;	/* blocks count */
	u_int32_t  e2fs_rbcount_lo;	/* reserved blocks count */
	u_int32_t  e2fs_fbcount_lo;	/* free blocks count */
	u_int32_t  e2fs_ficount;	/* free inodes count */
	u_int32_t  e2fs_first_dblock;	/* first data block */
	u_int32_t  e2fs_log_bsize;	/* block size = 1024*(2^e2fs_log_bsize) */
	u_int32_t  e2fs_log_fsize;	/* fragment size */
	u_int32_t  e2fs_bpg;		/* blocks per group */
	u_int32_t  e2fs_fpg;		/* frags per group */
	u_int32_t  e2fs_ipg;		/* inodes per group */
	u_int32_t  e2fs_mtime;		/* mount time */
	u_int32_t  e2fs_wtime;		/* write time */
	u_int16_t  e2fs_mnt_count;	/* mount count */
	u_int16_t  e2fs_max_mnt_count;	/* max mount count */
	u_int16_t  e2fs_magic;		/* magic number */
	u_int16_t  e2fs_state;		/* file system state */
	u_int16_t  e2fs_beh;		/* behavior on errors */
	u_int16_t  e2fs_minrev;		/* minor revision level */
	u_int32_t  e2fs_lastfsck;	/* time of last fsck */
	u_int32_t  e2fs_fsckintv;	/* max time between fscks */
	u_int32_t  e2fs_creator;	/* creator OS */
	u_int32_t  e2fs_rev;		/* revision level */
	u_int16_t  e2fs_ruid;		/* default uid for reserved blocks */
	u_int16_t  e2fs_rgid;		/* default gid for reserved blocks */
	/* EXT2_DYNAMIC_REV superblocks */
	u_int32_t  e2fs_first_ino;	/* first non-reserved inode */
	u_int16_t  e2fs_inode_size;	/* size of inode structure */
	u_int16_t  e2fs_block_group_nr;	/* block grp number of this sblk*/
	u_int32_t  e2fs_features_compat; /*  compatible feature set */
	u_int32_t  e2fs_features_incompat; /* incompatible feature set */
	u_int32_t  e2fs_features_rocompat; /* RO-compatible feature set */
	u_int8_t   e2fs_uuid[16];	/* 128-bit uuid for volume */
	char       e2fs_vname[16];	/* volume name */
	char       e2fs_fsmnt[64]; 	/* name mounted on */
	u_int32_t  e2fs_algo;		/* For comcate for dir */
        u_int8_t   e2fs_prealloc_blk;   /* number of blocks to try to preallocate */
        u_int8_t   e2fs_prealloc_dblk;  /* number of dirs to preallocate */
	u_int16_t  e2fs_reserved_ngdb;  /* # of reserved gd blocks for resize */
        u_int8_t   e2fs_journal_uuid[16]; /* uuid of journal superblock */
        u_int32_t  e2fs_journal_inum;   /* inode number of journal file */
        u_int32_t  e2fs_journal_dev;    /* device number of journal file */
        u_int32_t  e2fs_last_orphan;    /* start of list of inodes to delete */
        u_int32_t  e2fs_hash_seed[4];   /* HTREE hash seed */
        u_int8_t   e2fs_def_hash_ver;   /* default hash version to use */
        u_int8_t   e2fs_char_pad;
        u_int16_t  e2fs_desc_size;      /* size of group descriptor */
        u_int32_t  e2fs_def_mnt_opts;
        u_int32_t  e2fs_first_meta_bg;  /* first metablock block group */
        u_int32_t  e2fs_mkfs_time;      /* when the fs was created */
        u_int32_t  e2fs_jnl_blks[17];   /* backup of the journal inode */
        u_int32_t  e2fs_bcount_hi;      /* block count */
        u_int32_t  e2fs_rbcount_hi;     /* reserved blocks count */
        u_int32_t  e2fs_fbcount_hi;     /* free blocks count */
        u_int16_t  e2fs_min_extra_isize;/* all inodes have at least some bytes */
        u_int16_t  e2fs_want_extra_isize; /* new inodes should reserve some bytes */
        u_int32_t  e2fs_flags;          /* miscellaneous flags */
        u_int16_t  e2fs_raid_stride;    /* RAID stride */
        u_int16_t  e2fs_mmpintv;        /* number of seconds to wait in MMP checking */
        u_int64_t  e2fs_mmpblk;         /* block for multi-mount protection */
        u_int32_t  e2fs_raid_stripe_wid;/* blocks on all data disks (N * stride) */
        u_int8_t   e2fs_log_gpf;        /* FLEX_BG group size */ 
        u_int8_t   e2fs_char_pad2;
        u_int16_t  e2fs_pad;
        u_int64_t  e2fs_kbytes_written; /* number of lifetime kilobytes written */
	u_int32_t  reserved2[160];
};


/* Assume that user mode programs are passing in an ext2fs superblock, not
 * a kernel struct super_block.  This will allow us to call the feature-test
 * macros from user land. */
#define EXT2_SB(sb)	(sb)

/*
 * In-Memory Superblock
 */

struct m_ext2fs {
	struct ext2fs * e2fs;
	char e2fs_fsmnt[MAXMNTLEN];/* name mounted on */
	char e2fs_ronly;          /* mounted read-only flag */
	char e2fs_fmod;           /* super block modified flag */
	uint32_t e2fs_bsize;      /* Block size */
	uint32_t e2fs_bshift;     /* calc of logical block no */
	int32_t e2fs_bmask;       /* calc of block offset */
	int32_t e2fs_bpg;         /* Number of blocks per group */
	int64_t e2fs_qbmask;       /* = s_blocksize -1 */
	uint32_t e2fs_fsbtodb;     /* Shift to get disk block */
	uint32_t e2fs_ipg;        /* Number of inodes per group */
	uint32_t e2fs_ipb;        /* Number of inodes per block */
	uint32_t e2fs_itpg;       /* Number of inode table per group */
	uint32_t e2fs_fsize;      /* Size of fragments per block */
	uint32_t e2fs_fpb;        /* Number of fragments per block */
	uint32_t e2fs_fpg;        /* Number of fragments per group */
	uint32_t e2fs_dbpg;       /* Number of descriptor blocks per group */
	uint32_t e2fs_descpb;     /* Number of group descriptors per block */
	uint32_t e2fs_gdbcount;   /* Number of group descriptors */
	uint32_t e2fs_gcount;     /* Number of groups */
	uint32_t e2fs_first_inode;/* First inode on fs */
	int32_t  e2fs_isize;      /* Size of inode */
	uint32_t e2fs_mount_opt;
	uint32_t e2fs_blocksize_bits;
	uint32_t e2fs_total_dir;  /* Total number of directories */
	uint8_t	*e2fs_contigdirs;
	char e2fs_wasvalid;       /* valid at mount time */
	off_t e2fs_maxfilesize;
	struct ext2_gd *e2fs_gd;  /* Group Descriptors */

	struct mtx e2fs_rsv_lock;                /* Protect reservation window RB tree */
	struct ext2_rsv_win_tree e2fs_rsv_tree; /* Reservation window index */

        u_int8_t e2fs_log_gpf;    /* FLEX_BG group size */
        int      e2fs_descpbbits;
        struct ext4_flex_groups *e2fs_fg;

        u_int16_t e2fs_min_extra_isize;        /* all inodes have at least some bytes */
        u_int16_t e2fs_want_extra_isize;       /* new inodes should reserve some bytes */
};

/*
 * The second extended file system version
 */
#define E2FS_DATE		"95/08/09"
#define E2FS_VERSION		"0.5b"

/*
 * Revision levels
 */
#define E2FS_REV0		0	/* The good old (original) format */
#define E2FS_REV1		1 	/* V2 format w/ dynamic inode sizes */

#define E2FS_CURRENT_REV	E2FS_REV0
#define E2FS_MAX_SUPP_REV	E2FS_REV1

#define E2FS_REV0_INODE_SIZE 128

/*
 * compatible/incompatible features
 */
#define EXT2F_COMPAT_PREALLOC		0x0001
#define EXT2F_COMPAT_RESIZE		0x0010
#define EXT4F_COMPAT_IMAGIC_INODES      0x0002
#define EXT4F_COMPAT_HAS_JOURNAL        0x0004
#define EXT4F_COMPAT_EXT_ATTR           0x0008
#define EXT4F_COMPAT_DIR_INDEX          0x0020

#define EXT2F_ROCOMPAT_SPARSESUPER	0x0001
#define EXT2F_ROCOMPAT_LARGEFILE	0x0002
#define EXT2F_ROCOMPAT_BTREE_DIR	0x0004
#define EXT4F_ROCOMPAT_HUGE_FILE        0x0008
#define EXT4F_ROCOMPAT_GDT_CSUM         0x0010
#define EXT4F_ROCOMPAT_DIR_NLINK        0x0020
#define EXT4F_ROCOMPAT_EXTRA_ISIZE      0x0040

#define EXT2F_INCOMPAT_COMP		0x0001
#define EXT2F_INCOMPAT_FTYPE		0x0002
#define EXT4F_INCOMPAT_RECOVER          0x0004
#define EXT4F_INCOMPAT_JOURNAL_DEV      0x0008
#define EXT4F_INCOMPAT_META_BG          0x0010
#define EXT4F_INCOMPAT_EXTENTS          0x0040
#define EXT4F_INCOMPAT_64BIT            0x0080
#define EXT4F_INCOMPAT_MMP              0x0100
#define EXT4F_INCOMPAT_FLEX_BG          0X0200

/*
 * Features supported in this implementation
 *
 * We support the following REV1 features:
 * - EXT2F_ROCOMPAT_SPARSESUPER
 * - EXT2F_ROCOMPAT_LARGEFILE
 * - EXT2F_INCOMPAT_FTYPE
 */
#define EXT2F_COMPAT_SUPP		0x0000
#define EXT2F_ROCOMPAT_SUPP		(EXT2F_ROCOMPAT_SPARSESUPER \
					 | EXT2F_ROCOMPAT_LARGEFILE)
#define EXT2F_INCOMPAT_SUPP		EXT2F_INCOMPAT_FTYPE

/*
 * Features supported in ext4 read-only mode
 */
#define EXT4F_INCOMPAT_SUPP             (EXT2F_INCOMPAT_FTYPE \
                                         | EXT4F_INCOMPAT_EXTENTS \
                                         | EXT4F_INCOMPAT_FLEX_BG)
#define EXT4F_ROCOMPAT_SUPP             (EXT2F_ROCOMPAT_SPARSESUPER \
                                         | EXT2F_ROCOMPAT_LARGEFILE \
                                         | EXT2F_ROCOMPAT_BTREE_DIR \
                                         | EXT4F_ROCOMPAT_GDT_CSUM \
                                         | EXT4F_ROCOMPAT_DIR_NLINK \
                                         | EXT4F_ROCOMPAT_EXTRA_ISIZE \
                                         | EXT4F_ROCOMPAT_HUGE_FILE)

/*
 * Feature set definitions
 */
#define EXT2_HAS_COMPAT_FEATURE(sb,mask)			\
	( EXT2_SB(sb)->e2fs->e2fs_features_compat & htole32(mask) )
#define EXT2_HAS_RO_COMPAT_FEATURE(sb,mask)			\
	( EXT2_SB(sb)->e2fs->e2fs_features_rocompat & htole32(mask) )
#define EXT2_HAS_INCOMPAT_FEATURE(sb,mask)			\
	( EXT2_SB(sb)->e2fs->e2fs_features_incompat & htole32(mask) )

/*
 * Definitions of behavior on errors
 */
#define E2FS_BEH_CONTINUE		1	/* continue operation */
#define E2FS_BEH_READONLY		2	/* remount fs read only */
#define E2FS_BEH_PANIC			3	/* cause panic */
#define E2FS_BEH_DEFAULT		E2FS_BEH_CONTINUE

/*
 * OS identification
 */
#define E2FS_OS_LINUX		0
#define E2FS_OS_HURD		1
#define E2FS_OS_MASIX		2
#define E2FS_OS_FREEBSD		3
#define E2FS_OS_LITES		4

/*
 * File clean flags
 */
#define	E2FS_ISCLEAN			0x0001	/* Unmounted cleanly */
#define	E2FS_ERRORS			0x0002	/* Errors detected */

/* ext2 file system block group descriptor */

struct ext2_gd {
	u_int32_t ext2bgd_b_bitmap_lo;	/* blocks bitmap block */
	u_int32_t ext2bgd_i_bitmap_lo;	/* inodes bitmap block */
	u_int32_t ext2bgd_i_tables_lo;	/* inodes table block  */
	u_int16_t ext2bgd_nbfree_lo;	/* number of free blocks */
	u_int16_t ext2bgd_nifree_lo;	/* number of free inodes */
	u_int16_t ext2bgd_ndirs_lo;	/* number of directories */
        u_int16_t ext2bgd_flags;        /* EXT4_BG_flags */
#if 0
	u_int32_t reserved[2];
        u_int16_t ext2bgd_i_tables_unused_lo; /* number of unused inodes */
        u_int16_t ext2bgd_chksum;       /* crc16 checksum */
	u_int32_t ext2bgd_b_bitmap_hi;	/* blocks bitmap block MSB */
	u_int32_t ext2bgd_i_bitmap_hi;	/* inodes bitmap block MSB */
	u_int32_t ext2bgd_i_tables_hi;	/* inodes table block  MSB */
	u_int16_t ext2bgd_nbfree_hi;	/* number of free blocks MSB */
	u_int16_t ext2bgd_nifree_hi;	/* number of free inodes MSB */
	u_int16_t ext2bgd_ndirs_hi;	/* number of directories MSB */
        u_int16_t ext2bgd_i_tables_unused_hi; /* number of unused inodes MSB */
#endif
        u_int32_t reserved2[3];
};

/* EXT2FS metadatas are stored in little-endian byte order. These macros
 * helps reading these metadatas
 */

#define e2fs_cgload(old, new, size) memcpy((new), (old), (size));
#define e2fs_cgsave(old, new, size) memcpy((new), (old), (size));
/*
 * Macro-instructions used to manage several block sizes
 */
#define EXT2_MIN_BLOCK_SIZE		1024
#define	EXT2_MAX_BLOCK_SIZE		65536
#define EXT2_MIN_BLOCK_LOG_SIZE		  10
#if defined(_KERNEL)
# define EXT2_BLOCK_SIZE(s)		((s)->e2fs_bsize)
#else
# define EXT2_BLOCK_SIZE(s)		(EXT2_MIN_BLOCK_SIZE << (s)->e2fs_log_bsize)
#endif
#define	EXT2_ADDR_PER_BLOCK(s)		(EXT2_BLOCK_SIZE(s) / sizeof (uint32_t))
#if defined(_KERNEL)
# define EXT2_BLOCK_SIZE_BITS(s)	((s)->e2fs_blocksize_bits)
#else
# define EXT2_BLOCK_SIZE_BITS(s)	((s)->e2fs_log_bsize + 10)
#endif
#if defined(_KERNEL)
#define	EXT2_ADDR_PER_BLOCK_BITS(s)	(EXT2_SB(s)->s_addr_per_block_bits)
#define EXT2_INODE_SIZE(s)		(EXT2_SB(s)->e2fs_isize)
#define EXT2_FIRST_INO(s)		(EXT2_SB(s)->e2fs_first_inode)
#else
#define EXT2_INODE_SIZE(s)	(((s)->s_rev_level == E2FS_REV0) ? \
				 E2FS_REV0 : (s)->s_inode_size)
#define EXT2_FIRST_INO(s)	(((s)->s_rev_level == E2FS_REV0) ? \
				 E2FS_REV0 : (s)->e2fs_first_ino)
#endif

/*
 * Macro-instructions used to manage fragments
 */
#define EXT2_MIN_FRAG_SIZE		1024
#define	EXT2_MAX_FRAG_SIZE		4096
#define EXT2_MIN_FRAG_LOG_SIZE		  10
#if defined(_KERNEL)
# define EXT2_FRAG_SIZE(s)		(EXT2_SB(s)->e2fs_fsize)
# define EXT2_FRAGS_PER_BLOCK(s)	(EXT2_SB(s)->e2fs_fpb)
#else
# define EXT2_FRAG_SIZE(s)		(EXT2_MIN_FRAG_SIZE << (s)->e2fs_log_fsize)
# define EXT2_FRAGS_PER_BLOCK(s)	(EXT2_BLOCK_SIZE(s) / EXT2_FRAG_SIZE(s))
#endif

/*
 * Macro-instructions used to manage group descriptors
 */
#if defined(_KERNEL)
# define EXT2_BLOCKS_PER_GROUP(s)	(EXT2_SB(s)->e2fs_bpg)
# define EXT2_DESC_PER_BLOCK(s)		(EXT2_SB(s)->e2fs_descpb)
# define EXT2_DESC_PER_BLOCK_BITS(s)	(EXT2_SB(s)->s_desc_per_block_bits)
#else
# define EXT2_BLOCKS_PER_GROUP(s)	((s)->e2fs_bpg)
# define EXT2_DESC_PER_BLOCK(s)		(EXT2_BLOCK_SIZE(s) / sizeof (struct ext2_gd))

#endif

#endif	/* _LINUX_EXT4_FS_H */
