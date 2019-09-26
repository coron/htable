// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License version 2 as published
// by the Free Software Foundation.

#ifndef __prgmat_h__
#define __prgmat_h__

#define PRGA 0
#define PRGB 1
#define PRGC 2

void init_mprgmat(int ni,int n,int dmax);
byte get_mprgmatabc(int type,int i,int n);
byte get_mprgmati(int i);
void free_mprgmat(int ni,int n);

#endif
