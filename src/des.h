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

void des_encrypt(byte *in,byte *out,byte *key);

#endif
