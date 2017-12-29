// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License version 2 as published
// by the Free Software Foundation.

#include "common.h"

void polygen_share(int ii,byte *x,int n);
void polyRoy_share(int ii,byte *x,int n);
void des_encrypt_carlet(byte *in,byte *out,byte *key);
byte multtable(byte x,byte y);
void square_share(byte *a,byte *b,int n);
void multshare(byte *a,byte *b,byte *c,int n);
