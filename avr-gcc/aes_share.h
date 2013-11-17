#ifndef __aes_share_h__
#define __aes_share_h__

#include "aes.h"

void shiftrows_share(byte *stateshare[16],int n);
void mixcolumns_share(byte *stateshare[16],int n);
void addroundkey_share(byte *stateshare[16],byte *wshare[176],int round,int n);

double run_aes_share(byte in[16],byte out[16],byte key[16],int n,void (*subbyte_share_call)(byte *,int),int nt);

#endif
