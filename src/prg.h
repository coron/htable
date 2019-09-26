// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License version 2 as published
// by the Free Software Foundation.

#ifndef __prg_h__
#define __prg_h__



void init_prg(int d);
byte get_prg();
int get_prgcount();
void free_prg();

void init_robprg(int d,int n);
byte get_robprg(int n);
int get_robprgcount();
void free_robprg(int n);

void init_mprg(int rc,int ni,int ri,int n);
byte get_mprgc(int i);
byte get_mprgi(int i);
int get_mprgcount();
void free_mprg(int ni,int n);


#endif
