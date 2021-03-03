// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License version 2 as published
// by the Free Software Foundation.

#ifndef __aes_share_h__
#define __aes_share_h__

#include "aes.h"

#define TFLR 0
#define TILR 1

void shiftrows_share(byte *stateshare[16],int n);
void mixcolumns_share(byte *stateshare[16],int n);
void addroundkey_share(byte *stateshare[16],byte *wshare[176],int round,int n);

int run_aes_share(byte in[16],byte out[16],byte key[16],byte outex[16],int n,int nt,float base);
int run_aes_share_s(byte in[16],byte out[16],byte key[16],byte outex[16],int k,int ell,int nt,float base);

#endif
