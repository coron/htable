// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License version 2 as published
// by the Free Software Foundation.

#include "aes.h"

byte multtable(byte x,byte y);
void aes_rp(byte in[16],byte out[16],byte key[16]);
void subbyte_rp_share(byte *a,int n);
void multshare(byte *a,byte *b,byte *c,int n);
void subbyte_rp_share_func(byte *a,int n,void (*multshare_call)(byte *,byte *,byte *,int));
