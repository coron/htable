// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License version 2 as published
// by the Free Software Foundation.

#ifndef __share_h__
#define __share_h__

#include "common.h"

unsigned long xorshf96(void);
void init_randcount();
unsigned int get_randcount();
void set_randcount(unsigned int randc);

void share(byte x,byte a[],int n);
void refresh(byte a[],int n);
void encode(byte x,byte *a,int n);
byte decode(byte a[],int n);

void rotate_shuffling(byte *in,byte *dest,int delta,int ell);
void reshuffle(byte a[],int *ind,int ell);

void reshuffle_s(byte *a,int *ind,int k,int ell);
void encode_s(byte x,byte *a,int *ind,int k,int ell);
byte decode_s(byte *a,int *ind,int k,int ell);

void xor_gate_shuffling(byte *a,int ia,byte *b,int ib,byte *c,int *ic,int ell);
void xor_gate_shuffling_noind(byte *a,byte *b,byte *c,int ell);
void xor_gate_s(byte *a,int *ia,byte *b,int *ib,byte *c,int *ic,int k,int ell);

void multx_s(byte *a,int *ia,byte *b,int *ib,int k,int ell);
void mult3_s(byte *a,int *ia,byte *b,int *ib,int k,int ell);

#endif
