// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License version 2 as published
// by the Free Software Foundation.

#ifndef __aes_rp_prg_h__
#define __aes_rp_prg_h__

void subbyte_rp_share_flr(byte *a,int n);
void subbyte_rp_share_ilr(byte *a,int n);
void subbyte_rp_share_ilr2(byte *a,int n);
void subbyte_rp_share_flr_mprg(byte *a,int n);
void subbyte_rp_share_ilr_mprg(byte *a,int n);
int rprg_flr(int n);
int rprg_ilr(int n);
void subbyte_rp_share_flr_mprgmat(byte *a,int n);

void xor_loc(byte *a,byte *b,byte *c,int n);
void xor_loc32(byte *a,byte *b,byte *c,int n);

#endif
