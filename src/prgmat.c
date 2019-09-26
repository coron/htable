// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License version 2 as published
// by the Free Software Foundation.

#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "share.h"
#include "prgmat.h"

/*
static unsigned long x=123456789, y=362436069, z=521288629;
static unsigned int randcount=0;

unsigned long xorshf96(void) {   
  unsigned long t;
  randcount++;

  x ^= x << 16;
  x ^= x >> 5;
  x ^= x << 1;

  t = x;
  x = y;
  y = z;
  z = t ^ x ^ y; 
  return z;
}
*/

typedef struct
{
  int x,y;
  byte *r;
  byte *s;
  int flag;
  int m;
  int dmax;
} tprgmat;

void init_prgmat(tprgmat *prgmat,int dmax_)
{
  prgmat->dmax=dmax_;
  prgmat->m=0;
  prgmat->x=0;
  prgmat->y=0;
  prgmat->r=(byte *) malloc(dmax_*sizeof(byte));
  prgmat->s=(byte *) malloc(dmax_*sizeof(byte));
  prgmat->flag=0;
  prgmat->r[0]=xorshf96();
  prgmat->s[0]=xorshf96();
}

byte get_prgmat(tprgmat *prgmat)
{
  byte res;
  if(prgmat->flag==0)
    res=prgmat->r[prgmat->x] ^ prgmat->s[prgmat->y];
  else
    res=prgmat->r[prgmat->y] ^ prgmat->s[prgmat->x];

  if((prgmat->flag==0) && (prgmat->x!=prgmat->y))
  {
    prgmat->flag=1;
    return res;
  }

  prgmat->flag=0;
  
  prgmat->y++;
  if(prgmat->y>prgmat->m)
  {
    prgmat->y=0;
    prgmat->x++;
    prgmat->m++;
    if(prgmat->m>=prgmat->dmax)
    {
      fprintf(stderr,"prgmat: dmax too small\n");
      exit(0);
    }
      
    prgmat->r[prgmat->m]=xorshf96();
    prgmat->s[prgmat->m]=xorshf96();
  }
  return res;
}

void free_prgmat(tprgmat *prgmat)
{
  free(prgmat->r);
  free(prgmat->s);
}

typedef struct
{
  tprgmat *vabc;
  tprgmat *vi;
} tmprgmat;

tmprgmat mprgmat;

void init_mprgmat(int ni,int n,int dmax)
{
  mprgmat.vabc=malloc(3*(n-1)*sizeof(tprgmat));
  for(int i=0;i<3*(n-1);i++)
    init_prgmat(&mprgmat.vabc[i],dmax);

  mprgmat.vi=malloc(ni*sizeof(tprgmat));
  for(int i=0;i<ni;i++)
    init_prgmat(&mprgmat.vi[i],dmax);
  //printf("init_mprgmat\n");
}

byte get_mprgmatabc(int type,int i,int n)
{
  return get_prgmat(&mprgmat.vabc[type*(n-1)+i]);
}

byte get_mprgmati(int i)
{
  return get_prgmat(&mprgmat.vi[i]);
}

void free_mprgmat(int ni,int n)
{
  for(int i=0;i<3*(n-1);i++)
    free_prgmat(&mprgmat.vabc[i]);
  
  for(int i=0;i<ni;i++)
    free_prgmat(&mprgmat.vi[i]);

  free(mprgmat.vabc);
  free(mprgmat.vi);
  //printf("free_mprgmat\n");
}

/*
int main()
{
  tprgmat prgmat;
  init_prgmat(&prgmat,10);
  for(int i=0;i<30;i++)
  {
    printf("%d %d %d %d ",prgmat.x,prgmat.y,prgmat.flag,randcount);
    byte res=get_prgmat(&prgmat);
    printf("%d\n",res);
  }
  free_prgmat(&prgmat);
}

*/
