/*
 * Copyright 2010, Haiku Inc. All rights reserved.
 * This file may be used under terms of the MIT License.
 *
 * Authors:
 *      Janito V. Ferreira Filho
 *      Zheng Liu
 */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/conf.h>
#include <sys/stat.h>
#include <sys/hash.h>

#include <fs/ext4fs/ext4_hash.h>

/*
 * Constants for MD4Transform routine.
 */
#define S11 3
#define S12 7
#define S13 11
#define S14 19
#define S21 3
#define S22 5
#define S23 9
#define S24 13
#define S31 3
#define S32 9
#define S33 11
#define S34 15

/* F, G and H are basic MD4 functions.
 */
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (y)) | ((x) & (z)) | ((y) & (z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))

/* ROTATE_LEFT rotates x left n bits.
 */
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

/* FF, GG and HH are transformations for rounds 1, 2 and 3 */
/* Rotation is separate from addition to prevent recomputation */
#define FF(a, b, c, d, x, s) { \
    (a) += F ((b), (c), (d)) + (x); \
    (a) = ROTATE_LEFT ((a), (s)); \
  }
#define GG(a, b, c, d, x, s) { \
    (a) += G ((b), (c), (d)) + (x) + (u_int32_t)0x5a827999; \
    (a) = ROTATE_LEFT ((a), (s)); \
  }
#define HH(a, b, c, d, x, s) { \
    (a) += H ((b), (c), (d)) + (x) + (u_int32_t)0x6ed9eba1; \
    (a) = ROTATE_LEFT ((a), (s)); \
  }

/* 
 * MD4 basic transformation. Transforms state based on block.
 *
 * XXX: we just implement a half md4 algorithm because Linux uses
 * this algoirhtm. This function copies from kern/md4c.c file and
 * is modified according to Linux's implementation.
 */
u_int32_t
half_md4_transform(u_int32_t buf[4], u_int32_t in[8])
{
	u_int32_t a = buf[0], b = buf[1], c = buf[2], d = buf[3];

        /* Round 1 */
        FF (a, b, c, d, in[ 0], S11); /* 1 */
        FF (d, a, b, c, in[ 1], S12); /* 2 */
        FF (c, d, a, b, in[ 2], S13); /* 3 */
        FF (b, c, d, a, in[ 3], S14); /* 4 */
        FF (a, b, c, d, in[ 4], S11); /* 5 */
        FF (d, a, b, c, in[ 5], S12); /* 6 */
        FF (c, d, a, b, in[ 6], S13); /* 7 */
        FF (b, c, d, a, in[ 7], S14); /* 8 */

        /* Round 2 */
        GG (a, b, c, d, in[ 1], S21); /* 17 */
        GG (d, a, b, c, in[ 3], S22); /* 18 */
        GG (c, d, a, b, in[ 5], S23); /* 19 */
        GG (b, c, d, a, in[ 7], S24); /* 20 */
        GG (a, b, c, d, in[ 0], S21); /* 21 */
        GG (d, a, b, c, in[ 2], S22); /* 22 */
        GG (c, d, a, b, in[ 4], S23); /* 23 */
        GG (b, c, d, a, in[ 6], S24); /* 24 */

        /* Round 3 */
        HH (a, b, c, d, in[ 3], S31); /* 33 */
        HH (d, a, b, c, in[ 7], S32); /* 34 */
        HH (c, d, a, b, in[ 2], S33); /* 35 */
        HH (b, c, d, a, in[ 6], S34); /* 36 */
        HH (a, b, c, d, in[ 1], S31); /* 37 */
        HH (d, a, b, c, in[ 5], S32); /* 38 */
        HH (c, d, a, b, in[ 0], S33); /* 39 */
        HH (b, c, d, a, in[ 4], S34); /* 40 */

	buf[0] += a;
	buf[1] += b;
	buf[2] += c;
	buf[3] += d;

	return buf[1]; /* "most hashed" word */
}

/*
 * Tiny Encryption Algorithm.
 */
void
TEA_transform(u_int32_t buf[4], u_int32_t in[8])
{
        u_int32_t sum;
        u_int32_t x, y;
        u_int32_t a, b, c, d;
        int i;

        sum = 0;

        x = buf[0];
        y = buf[1];
        
        a = in[0];
        b = in[1];
        c = in[2];
        d = in[3];

        for (i = 0; i < 16; i++) {
                sum += 0x9E3779B9;
                x += ((y << 4) + a) ^ (y + sum) ^ ((y >> 5) + b);
                y += ((x << 4) + c) ^ (x + sum) ^ ((x >> 5) + d);
        }

        buf[0] += x;
        buf[1] += y;
}

/*
 * It does not implement signed version of str2hashbuf function in Haiku.
 * But in Linux, the difference between signed's and unsigned's just
 * is variable type.
 */
void
str2hashbuf_signed(const char *str, int len, u_int32_t *buf, int num)
{
        u_int32_t pad, val;
        int i, cnt, max, remain;
        const signed char *scp = (const signed char *)str;

        pad = (u_int32_t)len;
        pad = (pad << 8) | pad;
        pad = (pad << 16) | pad;

        max = num * 4;
        if (len > max)
                len = max;

        cnt = len / 4;

        for (i = 0; i < cnt; i++) {
                val = (pad << 8) | *(scp++);
                val = (val << 8) | *(scp++);
                val = (val << 8) | *(scp++);
                val = (val << 8) | *(scp++);
                buf[i] = val;
        }

        if (cnt < num) {
                remain = len % 4;
                val = pad;
                for (i = 0; i < remain; i++)
                        val = (val << 8) + *(scp++);

                buf[cnt] = val;

                for (i = cnt + 1; i < num; i++)
                        buf[i] = pad;
        }
}

/*
 * Refer Haiku's implementation. This implemention is MIT license.
 * So the kernel can not be contaminated.
 */
void
str2hashbuf_unsigned(const char *str, int len, u_int32_t *buf, int num)
{
        u_int32_t pad, val;
        int i, cnt, max, remain;
        const unsigned char *ucp = (const unsigned char *)str;

        pad = (u_int32_t)len;
        pad = (pad << 8) | pad;
        pad = (pad << 16) | pad;

        max = num * 4;
        if (len > max)
                len = max;

        cnt = len / 4;

        for (i = 0; i < cnt; i++) {
                val = (pad << 8) | *(ucp++);
                val = (val << 8) | *(ucp++);
                val = (val << 8) | *(ucp++);
                val = (val << 8) | *(ucp++);
                buf[i] = val;
        }

        if (cnt < num) {
                remain = len % 4;
                val = pad;
                for (i = 0; i < remain; i++)
                        val = (val << 8) + *(ucp++);

                buf[cnt] = val;

                for (i = cnt + 1; i < num; i++)
                        buf[i] = pad;
        }
}

u_int32_t
legacy_hash_signed(const char *name, int len)
{
        u_int32_t hash = 0x12a3fe2d;
        u_int32_t prev = 0x37abe8f9;
        u_int32_t next;
        const signed char *scp = (const signed char *)name;

        for (; len > 0; --len, scp++) {
                next = prev + (hash ^ (((int)*scp) * 7152373));

                if ((next & 0x80000000) != 0)
                        next -= 0x7fffffff;

                prev = hash;
                hash = next;
        }

        return (hash << 1);
}

u_int32_t
legacy_hash_unsigned(const char *name, int len)
{
        u_int32_t hash = 0x12a3fe2d;
        u_int32_t prev = 0x37abe8f9;
        u_int32_t next;
        const unsigned char *ucp = (const unsigned char *)name;

        for (; len > 0; --len, ucp++) {
                next = prev + (hash ^ (((int)*ucp) * 7152373));

                if ((next & 0x80000000) != 0)
                        next -= 0x7fffffff;

                prev = hash;
                hash = next;
        }

        return (hash << 1);
}
