/*-
 * Copyright (c) 1995 The University of Utah and
 * the Computer Systems Laboratory at the University of Utah (CSL).
 * All rights reserved.
 *
 * Permission to use, copy, modify and distribute this software is hereby
 * granted provided that (1) source code retains these copyright, permission,
 * and disclaimer notices, and (2) redistributions including binaries
 * reproduce the notices in supporting documentation, and (3) all advertising
 * materials mentioning features or use of this software display the following
 * acknowledgement: ``This product includes software developed by the
 * Computer Systems Laboratory at the University of Utah.''
 *
 * THE UNIVERSITY OF UTAH AND CSL ALLOW FREE USE OF THIS SOFTWARE IN ITS "AS
 * IS" CONDITION.  THE UNIVERSITY OF UTAH AND CSL DISCLAIM ANY LIABILITY OF
 * ANY KIND FOR ANY DAMAGES WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
 *
 * CSL requests users of this software to return to csl-dist@cs.utah.edu any
 * improvements that they make and grant CSL redistribution rights.
 *
 *      Utah $Hdr$
 * $FreeBSD: src/sys/fs/ext4fs/ext4_inode_cnv.c,v 1.1 2010/09/16 20:41:00 lz Exp $
 */

/*
 * routines to convert on disk ext2 inodes into inodes and back
 */
#include <sys/param.h>
#include <sys/systm.h>
#include <sys/lock.h>
#include <sys/stat.h>
#include <sys/vnode.h>

#include <fs/ext4fs/inode.h>
#include <fs/ext4fs/ext4fs.h>
#include <fs/ext4fs/ext4_extern.h>
#include <fs/ext4fs/ext4_dinode.h>
#include <fs/ext4fs/ext4_extents.h>

void
ext2_print_inode( in )
	struct inode *in;
{
	int i;
        struct ext4_extent_header *ehp;
        struct ext4_extent *ep;

	printf( "Inode: %5d", in->i_number);
	printf( /* "Inode: %5d" */
		" Type: %10s Mode: 0x%o Flags: 0x%x  Version: %d\n",
		"n/a", in->i_mode, in->i_flags, in->i_gen);
	printf( "User: %5lu Group: %5lu  Size: %lu\n",
		(unsigned long)in->i_uid, (unsigned long)in->i_gid,
		(unsigned long)in->i_size);
	printf( "Links: %3d Blockcount: %lld\n",
		in->i_nlink, in->i_blocks);
	printf( "ctime: 0x%x", in->i_ctime);
	printf( "atime: 0x%x", in->i_atime);
	printf( "mtime: 0x%x", in->i_mtime);
	printf( "BLOCKS: ");
	for(i=0; i < (in->i_blocks <= 24 ? ((in->i_blocks+1)/2): 12); i++)
		printf("%d ", in->i_db[i]);
        printf( "\n");

        printf( "Extents:\n");
        ehp = (struct ext4_extent_header *)in->i_db;
        printf( "Header (magic 0x%x entries %d max %d depth %d gen %d)\n",
                ehp->eh_magic, ehp->eh_ecount, ehp->eh_max, ehp->eh_depth, ehp->eh_gen);
        ep = (struct ext4_extent *)((char *)(in->i_db) + sizeof(struct ext4_extent_header));
        printf( "Index (blk %d len %d start_lo %d start_hi %d)\n",
                ep->e_blk, ep->e_len, ep->e_start_lo, ep->e_start_hi);
	printf("\n");
}

/*
 *	raw ext2 inode to inode
 */
void
ext2_ei2i(ei, ip)
	struct ext2fs_dinode *ei;
	struct inode *ip;
{
        int i;

	ip->i_nlink = ei->e2di_nlink;
	/* Godmar thinks - if the link count is zero, then the inode is
	   unused - according to ext2 standards. Ufs marks this fact
	   by setting i_mode to zero - why ?
	   I can see that this might lead to problems in an undelete.
	*/
	ip->i_mode = ei->e2di_nlink ? ei->e2di_mode : 0;
	ip->i_size = ei->e2di_size_lo;
	if (S_ISREG(ip->i_mode))
		ip->i_size |= ((u_int64_t)ei->e2di_size_high) << 32;
	ip->i_atime = ei->e2di_atime;
	ip->i_mtime = ei->e2di_mtime;
	ip->i_ctime = ei->e2di_ctime;
        ip->i_flags = ei->e2di_flags;   /* we need to entire flags to check new features */
        ip->i_gen = ei->e2di_gen;
#if 0
	ip->i_flags = 0;
	ip->i_flags |= (ei->e2di_flags & EXT2_APPEND) ? SF_APPEND : 0;
	ip->i_flags |= (ei->e2di_flags & EXT2_IMMUTABLE) ? SF_IMMUTABLE : 0;
	ip->i_flags |= (ei->e2di_flags & EXT2_NODUMP) ? UF_NODUMP : 0;
#endif
        if (ip->i_e2fs->e2fs->e2fs_features_incompat & EXT4F_ROCOMPAT_HUGE_FILE)
                ip->i_blocks = ((int64_t)(ei->e2di_nblock_high)) << 32 | ei->e2di_nblock_lo;
        else
                ip->i_blocks = ei->e2di_nblock_lo;
	ip->i_gen = ei->e2di_gen;
	ip->i_uid = ei->e2di_uid;
	ip->i_gid = ei->e2di_gid;
	/* XXX use memcpy */
	for(i = 0; i < NDADDR; i++)
		ip->i_db[i] = ei->e2di_blocks[i];
	for(i = 0; i < NIADDR; i++)
		ip->i_ib[i] = ei->e2di_blocks[EXT2_NDIR_BLOCKS + i];
}

/*
 *	inode to raw ext2 inode
 */
void
ext2_i2ei(ip, ei)
	struct inode *ip;
	struct ext2fs_dinode *ei;
{
	int i;

	ei->e2di_mode = ip->i_mode;
        ei->e2di_nlink = ip->i_nlink;
	/* 
	   Godmar thinks: if dtime is nonzero, ext2 says this inode
	   has been deleted, this would correspond to a zero link count
	 */
	ei->e2di_dtime = ei->e2di_nlink ? 0 : ip->i_mtime;
	ei->e2di_size_lo = ip->i_size;
	if (S_ISREG(ip->i_mode))
		ei->e2di_size_high = ip->i_size >> 32;
	ei->e2di_atime = ip->i_atime;
	ei->e2di_mtime = ip->i_mtime;
	ei->e2di_ctime = ip->i_ctime;
	ei->e2di_flags = ip->i_flags;
	ei->e2di_flags = 0;
	ei->e2di_flags |= (ip->i_flags & SF_APPEND) ? EXT2_APPEND: 0;
	ei->e2di_flags |= (ip->i_flags & SF_IMMUTABLE) ? EXT2_IMMUTABLE: 0;
	ei->e2di_flags |= (ip->i_flags & UF_NODUMP) ? EXT2_NODUMP: 0;
	ei->e2di_nblock_lo = ip->i_blocks;
	ei->e2di_gen = ip->i_gen;
	ei->e2di_uid = ip->i_uid;
	ei->e2di_gid = ip->i_gid;
	/* XXX use memcpy */
	for(i = 0; i < NDADDR; i++)
		ei->e2di_blocks[i] = ip->i_db[i];
	for(i = 0; i < NIADDR; i++)
		ei->e2di_blocks[EXT2_NDIR_BLOCKS + i] = ip->i_ib[i];
}
