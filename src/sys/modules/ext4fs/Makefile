# $FreeBSD: src/sys/modules/ext4fs/Makefile,v 1.7 2010/09/16 20:23:54 lz Exp $

.PATH:	${.CURDIR}/../../fs/ext4fs
KMOD=	ext4fs
SRCS=	opt_ddb.h opt_quota.h opt_suiddir.h vnode_if.h \
	ext4_alloc.c ext4_balloc.c ext4_bmap.c ext4_extents.c \
        ext4_inode.c ext4_inode_cnv.c ext4_lookup.c ext4_subr.c \
        ext4_vfsops.c ext4_vnops.c ext4_htree.c ext4_hash.c

.include <bsd.kmod.mk>
