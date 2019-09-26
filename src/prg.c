// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License version 2 as published
// by the Free Software Foundation.

#include "common.h"
#include "share.h"
#include "aes.h"
#include "aes_rp.h"
#include "prg.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void mult_gf16(byte a0,byte a1,byte b0,byte b1,byte *c0,byte *c1)
{
  byte a1b1=multtable(a1,b1);
  *c0=multtable(a0,b0) ^ multtable(32,a1b1);
  *c1=multtable(a1,b0) ^ multtable(a0,b1) ^ a1b1;
}

void test_mult_gf16()
{
  // see file gf.sage
  byte c0,c1;
  mult_gf16(129,37,23,13,&c0,&c1);
  assert(c0==137);
  assert(c1==200);
}

void eval_poly_gf16(byte *pol,int d,byte x0,byte x1,byte *r0,byte *r1)
{
  int i;
  *r0=0;*r1=0;
  for(i=d-1;i>=0;i--)
  {
    byte t0,t1;
    mult_gf16(*r0,*r1,x0,x1,&t0,&t1);
    *r0=t0 ^ pol[2*i];
    *r1=t1 ^ pol[2*i+1];
  }
}

void test_eval_poly_gf16()
{
  // see file gf.sage
  byte pol[6]={23,54,18,32,231,27};
  byte x0=67,x1=12;
  byte r0,r1;
  eval_poly_gf16(pol,2,x0,x1,&r0,&r1);
  assert(r0==180);
  assert(r1==113);
}

typedef struct
{
  int prgcount;
  int dprg;
  byte *pol;
  int prgflag;
  byte r0,r1;
} tprg;

tprg prg;

void init_prg_p(tprg *prg,int d)
{
  prg->dprg=d;
  prg->pol=(byte *) malloc(2*d*sizeof(byte));
  prg->prgcount=0;
  for(int i=0;i<2*d;i++)
    prg->pol[i]=xorshf96();
  prg->prgflag=0;
}

void init_prg(int d)
{
  init_prg_p(&prg,d);
}

byte get_prg_p(tprg *prg)
{
  if (prg->prgflag==1)
  {
    prg->prgflag=0;
    return prg->r1;
  }
  eval_poly_gf16(prg->pol,prg->dprg,prg->prgcount & 255,prg->prgcount >> 8,&prg->r0,&prg->r1);
  prg->prgcount++;
  prg->prgflag=1-prg->prgflag;
  return prg->r0;
}

byte get_prg()
{
  return get_prg_p(&prg);
}

int get_prgcount()
{
  return prg.prgcount*2;
}

void free_prg()
{
  free(prg.pol);
}

void test_prg()
{
  init_prg(10);
  for(int i=0;i<100;i++)
    printf("%d ",get_prg());
}

typedef struct
{
  tprg *vprg;
} trobprg;

trobprg robprg;

void init_robprg(int d,int n)
{
  robprg.vprg=malloc(n*sizeof(tprg));
  for(int i=0;i<n;i++)
    init_prg_p(&robprg.vprg[i],d);
}

byte get_robprg(int n)
{
  byte r=0;
  for(int i=0;i<n;i++)
    r^=get_prg_p(&robprg.vprg[i]);
  return r;
}

int get_robprgcount()
{
  return robprg.vprg[0].prgcount*2;
}

void free_robprg(int n)
{
  for(int i=0;i<n;i++)
    free(robprg.vprg[i].pol);
  free(robprg.vprg);
}
 
typedef struct
{
  int prgcount;
  tprg *vprgc;
  tprg *vprgi;
} tmprg;

tmprg mprg;

void init_mprg(int rc,int ni,int ri,int n)
{
  mprg.prgcount=0;
  mprg.vprgc=malloc((n-1)*sizeof(tprg));

  for(int i=0;i<n-1;i++)
    init_prg_p(&mprg.vprgc[i],rc);
  
  mprg.vprgi=malloc(ni*sizeof(tprg));
  for(int i=0;i<ni;i++)
    init_prg_p(&mprg.vprgi[i],ri);
}

void free_mprg(int ni,int n)
{
  for(int i=0;i<n-1;i++)
    free(mprg.vprgc[i].pol);

  free(mprg.vprgc);

  for(int i=0;i<ni;i++)
    free(mprg.vprgi[i].pol);
  free(mprg.vprgi);
}
  
byte get_mprgc(int i)
{
  mprg.prgcount++;
  return get_prg_p(&mprg.vprgc[i]);
}

byte get_mprgi(int i)
{
  mprg.prgcount++;
  return get_prg_p(&mprg.vprgi[i]);
}

int get_mprgcount()
{
  return mprg.prgcount;
}

/*
int main()
{
  test_mult_gf16();
  test_eval_poly_gf16();
  test_prg();
}
*/
