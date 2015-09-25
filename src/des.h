// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License version 2 as published
// by the Free Software Foundation.

#ifndef __des_h__
#define __des_h__

#include "common.h"

extern byte sbox[512];
extern int ipperm[64];
extern int ipinvperm[64];
extern int eperm[64];
extern int pperm[32];
extern int pc1perm[64];
extern int pc2perm[64];

void perm(byte *in,byte *out,int *dperm,int n);
void xorbyte(byte *data,byte *x,int nb);
void keyexpansion(byte *w,byte *key);
int run_des(byte *in,byte *out,byte *key,int nt);
void des_encrypt(byte *in,byte *out,byte *key);

#endif
