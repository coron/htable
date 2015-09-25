// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License version 2 as published
// by the Free Software Foundation.

#include "des_share.h"
#include "des.h"
#include "share.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>

void fsbox_share(byte *datashare[8],int n,void (*sbox_call)(int,byte *,int))
{
  int i,j;
  
  for(i=0;i<8;i++)
  {
    for(j=0;j<n;j++)
      datashare[i][j]>>=2;
    sbox_call(i,datashare[i],n);
  }

  for(i=0;i<4;i++)
  {
    for(j=0;j<n;j++)
      datashare[i][j]=(datashare[i*2+1][j] & 15) | ((datashare[i*2][j] & 15) << 4);
  }
}


void f_share(byte *inshare[4],byte *outshare[4],byte *wshare[128],int round,int n,void (*sbox_call)(int,byte *,int))
{
  int i,j;
  byte temp[8];
  byte in[4],out[4];
  byte *tempshare[8];
  
  for(i=0;i<8;i++)
    tempshare[i]=malloc(n*sizeof(byte));

  for(i=0;i<n;i++)
  {
    for(j=0;j<4;j++)
      in[j]=inshare[j][i];
    perm(in,temp,eperm,64);
    for(j=0;j<8;j++)
      tempshare[j][i]=temp[j];
  }
  
  for(j=0;j<8;j++)
    for(i=0;i<n;i++)
      tempshare[j][i]^=wshare[(round-1)*8+j][i];

  fsbox_share(tempshare,n,sbox_call);

  for(i=0;i<n;i++)
  {
    for(j=0;j<4;j++)
      temp[j]=tempshare[j][i];
    perm(temp,out,pperm,32);
    for(j=0;j<4;j++)
      outshare[j][i]=out[j];
  }

  for(i=0;i<8;i++)
    free(tempshare[i]);
}

void fround_share(byte *lshare[4],byte *rshare[4],byte *wshare[128],int round,int n,void (*sbox_call)(int,byte *,int))
{
  int i;
  byte temp[4],r[4],l[4];
  byte *tempshare[4];

  for(i=0;i<4;i++)
    tempshare[i]=malloc(n*sizeof(byte));

  f_share(rshare,tempshare,wshare,round,n,sbox_call);

  for(i=0;i<4;i++)
  {
    xorbyte(tempshare[i],lshare[i],n);
    memcpy(lshare[i],rshare[i],n);
    memcpy(rshare[i],tempshare[i],n);
  }
  for(i=0;i<4;i++)
    free(tempshare[i]);
}
  

void des_share_subkeys(byte *in,byte *out,byte *wshare[128],int n,void (*sbox_call)(int,byte *,int))
{
  int i;
  byte temp[8],l[4],r[4];
 
  perm(in,temp,ipperm,64);

  memcpy(l,temp,4);
  memcpy(r,temp+4,4);

  byte *lshare[8],*rshare[8];

  for(i=0;i<4;i++)
  {
    lshare[i]=(byte*) malloc(n*sizeof(byte));
    rshare[i]=(byte*) malloc(n*sizeof(byte));
    share(l[i],lshare[i],n);
    share(r[i],rshare[i],n);
    refresh(lshare[i],n);
    refresh(rshare[i],n);
  }  

  for(i=1;i<=16;i++)
    fround_share(lshare,rshare,wshare,i,n,sbox_call);

  for(i=0;i<4;i++)
  {
    l[i]=decode(lshare[i],n);
    r[i]=decode(rshare[i],n);
    free(lshare[i]);
    free(rshare[i]);
  }

  memcpy(temp,r,4);
  memcpy(temp+4,l,4);
  perm(temp,out,ipinvperm,64);
}

int run_des_share(byte *in,byte *out,byte *key,int n,void (*sbox_call)(int,byte *,int),int nt)
{
  int i;
  byte w[128];
  byte *wshare[176];
  clock_t start,end;

  keyexpansion(w,key);

  for(i=0;i<128;i++)
  {
    wshare[i]=(byte *) malloc(n*sizeof(byte));
    share(w[i],wshare[i],n);
    refresh(wshare[i],n);
  }

  start=clock();
  for(i=0;i<nt;i++)
    des_share_subkeys(in,out,wshare,n,sbox_call);
  end=clock();

  for(i=0;i<128;i++)
    free(wshare[i]);

  return (int) (end-start);
}
