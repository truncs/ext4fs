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
 * $FreeBSD: src/sys/fs/ext4fs/ext4_htree.c,v 0.1 2010/09/16 20:39:00 lz Exp $
 */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/conf.h>
#include <sys/vnode.h>
#include <sys/stat.h>
#include <sys/buf.h>
#include <sys/hash.h>

#include <fs/ext4fs/inode.h>
#include <fs/ext4fs/ext4_mount.h>
#include <fs/ext4fs/ext4fs.h>
#include <fs/ext4fs/fs.h>
#include <fs/ext4fs/ext4_extern.h>
#include <fs/ext4fs/ext4_dinode.h>
#include <fs/ext4fs/ext4_dir.h>
#include <fs/ext4fs/ext4_htree.h>
#include <fs/ext4fs/ext4_hash.h>

static int ext4_get_limit(struct dirindex_entry *);
static int ext4_root_limit(struct inode *, int);
static int ext4_node_limit(struct inode *);
static int ext4_dirindex_hash(char *, int, struct dirindex_hash_info *);
static int ext4_dirindex_probe(struct vnode *, char *, int,
                struct dirindex_hash_info *, struct dirindex_frame *);
static int ext4_get_next_block(struct vnode *, u_int32_t, struct dirindex_frame *,
                struct dirindex_frame *, u_int32_t *);

int
ext4_is_dirindex(struct inode *ip)
{
        struct ext2fs *fs;

        fs = ip->i_e2fs->e2fs;
        if ((fs->e2fs_features_compat & EXT4F_COMPAT_DIR_INDEX) &&
            (ip->i_flags & EXT4_INDEX))
                return (1);
        else
                return (0);
}

static int
ext4_get_limit(struct dirindex_entry *ep)
{
        return ((struct dirindex_countlimit *)(ep))->di_limit;
}

static int
ext4_node_limit(struct inode *ip)
{
        struct m_ext2fs *fs;
        int space;

        fs = ip->i_e2fs;
        space = fs->e2fs_bsize - EXT2_DIR_REC_LEN(0);
        return (space / sizeof(struct dirindex_entry));
}

static int
ext4_root_limit(struct inode *ip, int len)
{
        struct m_ext2fs *fs;
        int space;

        fs = ip->i_e2fs;
        space = fs->e2fs_bsize - EXT2_DIR_REC_LEN(1) -
            EXT2_DIR_REC_LEN(2) - len;
        return (space / sizeof(struct dirindex_entry));
}

static int
ext4_dirindex_hash(char *name, int namelen, struct dirindex_hash_info *hp)
{
        char *p;
        int i;
        u_int32_t in[8], buf[4];
        u_int32_t hash = 0, minhash = 0;

        /* initialize default hash value */
        buf[0] = 0x67452301;
        buf[1] = 0xefcdab89;
        buf[2] = 0x98badcfe;
        buf[3] = 0x10325476;

        if (hp->di_seed) {
                for (i = 0; i < 4; i++)
                        if (hp->di_seed[i])
                                break;
                if (i < 4)
                        memcpy(buf, hp->di_seed, sizeof(buf));
        }

        switch (hp->di_hashversion) {
        case DIRINDEX_HASH_HALF_MD4:
                p = name;
                while (namelen > 0) {
                        str2hashbuf_signed(p, namelen, in, 8);
                        half_md4_transform(buf, in);
                        namelen -= 32;
                        p += 32;
                }
                minhash = buf[2];
                hash = buf[1];
                break;
        case DIRINDEX_HASH_HALF_MD4_UNSIGNED:
                p = name;
                while (namelen > 0) {
                        str2hashbuf_unsigned(p, namelen, in, 8);
                        half_md4_transform(buf, in);
                        namelen -= 32;
                        p += 32;
                }
                minhash = buf[2];
                hash = buf[1];
                break;
        case DIRINDEX_HASH_LEGACY:
                hash = legacy_hash_signed(name, namelen);
                break;
        case DIRINDEX_HASH_LEGACY_UNSIGNED:
                hash = legacy_hash_unsigned(name, namelen);
                break;
        case DIRINDEX_HASH_TEA:
                p = name;
                while (namelen > 0) {
                        str2hashbuf_signed(p, namelen, in, 4);
                        TEA_transform(buf, in);
                        name -= 16;
                        p += 16;
                }
                minhash = buf[1];
                hash = buf[0];
                break;
        case DIRINDEX_HASH_TEA_UNSIGNED:
                p = name;
                while (namelen > 0) {
                        str2hashbuf_unsigned(p, namelen, in, 4);
                        TEA_transform(buf, in);
                        name -= 16;
                        p += 16;
                }
                minhash = buf[1];
                hash = buf[0];
                break;
        default:
                hp->di_hash = 0;
                return (-1);
        }

        hash = hash & ~1;
        if (hash == (0x7fffffff << 1))
                hash = (0x7fffffff - 1) << 1;
        hp->di_minhash = minhash;
        hp->di_hash = hash;

        return (0);
}

static int
ext4_dirindex_probe(struct vnode *vp, char *name, int namelen,
    struct dirindex_hash_info *hinfop, struct dirindex_frame *fp)
{
        struct ext2fs *fs;
        struct buf *bp = NULL;
        struct dirindex_entry *entp, *p, *q, *m, *at;
        struct dirindex_root *rp;
        int error;
        u_int32_t hash;
        int ind, cnt;

        fs = VTOI(vp)->i_e2fs->e2fs;

        if ((error = ext2_blkatoff(vp, (off_t)0, NULL, &bp)) != 0)
                return (error);

        rp = (struct dirindex_root *)bp->b_data;
        if (rp->di_info.hash_version != DIRINDEX_HASH_TEA &&
            rp->di_info.hash_version != DIRINDEX_HASH_HALF_MD4 &&
            rp->di_info.hash_version != DIRINDEX_HASH_LEGACY)
                goto err;

        hinfop->di_hashversion = rp->di_info.hash_version;
        if (hinfop->di_hashversion <= DIRINDEX_HASH_TEA)
                hinfop->di_hashversion += 3; /* XXX: In Linux it use 3 */
        hinfop->di_seed = fs->e2fs_hash_seed;

        if (name != NULL)
                ext4_dirindex_hash(name, namelen, hinfop);

        hash = hinfop->di_hash;

        if (rp->di_info.unused & 1)
                goto err;

        if ((ind = rp->di_info.ind_levels) > 1)
                goto err;

        entp = (struct dirindex_entry *)(((char *)&rp->di_info) +
            rp->di_info.info_len);

        if (ext4_get_limit(entp) !=
            ext4_root_limit(VTOI(vp), rp->di_info.info_len))
                goto err;

        while (1) {
                cnt = ((struct dirindex_countlimit *)(entp))->di_count;
                if (cnt == 0 || cnt > ext4_get_limit(entp))
                        goto err;

                p = entp + 1;
                q = entp + cnt - 1;
                while (p <= q) {
                        m = p + (q - p) / 2;
                        if (m->di_hash > hash)
                                q = m - 1;
                        else
                                p = m + 1;
                }

                at = p - 1;
                memcpy(&fp->di_ent, entp, sizeof(struct dirindex_entry));
                memcpy(&fp->di_at, at, sizeof(struct dirindex_entry));
                if (!ind--) {
                        if (bp != NULL)
                                brelse(bp);
                        return (0);
                }
                if (bp != NULL) {
                        brelse(bp);
                        bp = NULL;
                }
                if ((error = ext2_blkatoff(vp, (entp->di_blk & 0x00ffffff),
                    NULL, &bp)))
                        return (error);

                at = entp = ((struct dirindex_node *)bp->b_data)->di_ent;
                if (ext4_get_limit(entp) != ext4_node_limit(VTOI(vp)))
                        goto err;

                fp++;
        }

        if (bp != NULL)
                brelse(bp);
        return (0);

err:
        if (bp != NULL)
                brelse(bp);
        return (DIRINDEX_BAD_DIR);
}

static int
ext4_get_next_block(struct vnode *vp, u_int32_t hash, struct dirindex_frame *fp,
    struct dirindex_frame *frame, u_int32_t *starthash)
{
        struct dirindex_frame *p;
        struct dirindex_entry *ep, *atp;
        int cnt;
        int nframe = 0;
        u_int32_t bhash;
        struct buf *bp = NULL;
        int error;

        p = fp;

        while (1) {
                cnt = ((struct dirindex_countlimit *)&p->di_ent)->di_count;
                atp = &p->di_at;
                ep = &p->di_ent;
                if (++atp < ep + cnt)
                        break;

                if (p == frame)
                        return (0);
                nframe++;
                p--;
        }

        bhash = p->di_at.di_hash;
        if ((hash & 1) == 0)
                if ((bhash & ~1) != hash)
                        return (0);

        while (nframe--) {
                if ((error = ext2_blkatoff(vp, p->di_at.di_blk, NULL, &bp)))
                        return (error);
                p++;
                if (bp != NULL) {
                        brelse(bp);
                        bp = NULL;
                }
                memcpy(&p->di_at, ((struct dirindex_node *)bp->b_data)->di_ent,
                    sizeof(struct dirindex_entry));
                memcpy(&p->di_ent, ((struct dirindex_node *)bp->b_data)->di_ent,
                    sizeof(struct dirindex_entry));
        }

        return (1);
}

int
ext4_dirindex_lookup(struct vnode *vp, char *name, int namelen,
    doff_t *offp, struct buf **bpp, doff_t *prevoffp)
{
        struct dirindex_hash_info hinfo;
        struct dirindex_frame *fp, frame[2];
        struct buf *bp = NULL;
        struct ext2fs_direct_2 *ep, *top;
        u_int32_t hash;
        u_int32_t blk;
        int error, ret = 0;
        int entryoffset = 0;

        memset(&hinfo, 0, sizeof(hinfo));

        if (namelen > 2 || name[0] != '.' ||
            (name[1] != '.' && name[1] != '\0')) {
                if ((ext4_dirindex_probe(vp,
                    name, namelen, &hinfo, frame)) != 0)
                        return (ENOENT);
                fp = frame;
        } else {
                fp = frame;
                fp->di_at.di_blk = 0;
        }

        hash = hinfo.di_hash;

        blk = fp->di_at.di_blk & 0x00ffffff;
        if ((error = ext2_blkatoff(vp, blk * VTOI(vp)->i_e2fs->e2fs_bsize, NULL, &bp)))
                return (EIO);

        do {
                ep = (struct ext2fs_direct_2 *)bp->b_data;
                top  = (struct ext2fs_direct_2 *)((char *)ep +
                    VTOI(vp)->i_e2fs->e2fs_bsize - EXT2_DIR_REC_LEN(0));

                while (ep < top) {

                        /* TODO: need to check directory if needed */

                        if (ep->e2d_ino != 0 && namelen == ep->e2d_namlen &&
                            !bcmp(name, ep->e2d_name, (unsigned)namelen)) {
                                *offp = entryoffset;
                                *bpp = bp;
                                return (0);
                        }
                        entryoffset += ep->e2d_reclen;
                        ep = (struct ext2fs_direct_2 *)((char *)bp->b_data + entryoffset);
                }

                if (bp != NULL)
                        brelse(bp);

                ret = ext4_get_next_block(vp, hash, fp, frame, NULL);
                if (ret < 0)
                        return (EIO);
        } while (ret == 1);

        return (ENOENT);        
}
