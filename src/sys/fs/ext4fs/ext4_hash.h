/*
 * Copyright 2010, Haiku Inc. All rights reserved.
 * This file may be used under terms of the MIT License.
 *
 * Authors:
 *      Janito V. Ferreira Filho
 *      Zheng Liu
 */

#ifndef _FS_EXT4FS_EXT4_HASH_H_
#define _FS_EXT4FS_EXT4_HASH_H_

/* ext4_hash.c */
u_int32_t half_md4_transform(u_int32_t [], u_int32_t []);
void TEA_transform(u_int32_t [], u_int32_t []);
void str2hashbuf_signed(const char *, int, u_int32_t *, int);
void str2hashbuf_unsigned(const char *, int, u_int32_t *, int);
u_int32_t legacy_hash_signed(const char *, int);
u_int32_t legacy_hash_unsigned(const char *, int);

#endif /* !_FS_EXT4FS_EXT4_HASH_H_ */
