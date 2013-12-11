#ifndef __aes_share_h__
#define __aes_share_h__

#include "aes.h"

void shiftrows_share(byte *stateshare[16],uint8_t n);
void mixcolumns_share(byte *stateshare[16],uint8_t n);
void addroundkey_share(byte *stateshare[16],byte *wshare[176],uint8_t round,uint8_t n);

double run_aes_share(byte in[16],byte out[16],byte key[16],uint8_t n,void (*subbyte_share_call)(byte *,uint8_t),uint8_t nt);

#endif
