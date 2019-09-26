// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License version 2 as published
// by the Free Software Foundation.

#include "aes_rp.h"
#include "aes.h"
#include "share.h"
#include "prg.h"
#include "prgmat.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void locality_refresh(byte *a,int n)
{
  for(int i=0;i<n-1;i++)
  {
    byte r=get_robprg(n);
    a[n-1]^=r ^ a[i];
    a[i]=r;
  }
}

void locality_refresh_mprg(byte *a,int n)
{
  for(int i=0;i<n-1;i++)
  {
    byte r=get_mprgc(i);
    a[n-1]^=r ^ a[i];
    a[i]=r;
  }
}

void locality_refresh_mprgmatabc(byte *a,int type,int n)
{
  for(int i=0;i<n-1;i++)
  {
    byte r=get_mprgmatabc(type,i,n);
    a[n-1]^=r ^ a[i];
    a[i]=r;
  }
}

void xor_loc(byte *a,byte *b,byte *c,int n)
{
  locality_refresh_mprgmatabc(a,PRGA,n);
  locality_refresh_mprgmatabc(b,PRGB,n);
  for(int i=0;i<n;i++)
    c[i]=a[i] ^ b[i];
  locality_refresh_mprgmatabc(c,PRGC,n);
}

void xor_loc32(byte *a,byte *b,byte *c,int n)
{
  locality_refresh_mprgmatabc(a,PRGA,n);
  locality_refresh_mprgmatabc(b,PRGB,n);
  for(int i=0;i<n;i++)
    c[i]=multx(a[i]) ^ a[i] ^ multx(b[i]);
  locality_refresh_mprgmatabc(c,PRGC,n);
}

void multshare_prg(byte *a,byte *b,byte *c,int n)
{
  int i,j; 
  for(i=0;i<n;i++)
    c[i]=multtable(a[i],b[i]);

  for(i=0;i<n;i++)
  {
    for(j=i+1;j<n;j++)
    {
      byte tmp=get_robprg(n); //rand();
      byte tmp2=(tmp ^ multtable(a[i],b[j])) ^ multtable(a[j],b[i]);
      c[i]^=tmp;
      c[j]^=tmp2;
    }
  }
}

// multshare with final locality refresh
void multshare_flr(byte *a,byte *b,byte *c,int n)
{
  multshare_prg(a,b,c,n);
  locality_refresh(c,n);
}

// subbyte with initial and final locality refresh.
void subbyte_rp_share_flr(byte *a,int n)
{
  locality_refresh(a,n); 
  subbyte_rp_share_func(a,n,multshare_flr);
}

int max(int a,int b)
{
  if (a>b) return a;
  return b;
}

int rprg_flr(int n)
{
  int loc;
  if((n%2)==0)
    loc=n*n/4+5*n/2-3;
  else
    loc=(n*n-1)/4+5*(n-1)/2;
  loc=max(loc,4*(n-1));
  return (n-1)*loc;
}

int rprg_ilr(int n)
{
  int loc=4*(n-1); // =max(4*(n-1),4*n-5)
  return (n-1)*loc;
}

// multshare with internal locality refresh
void multshare_ilr(byte *a,byte *b,byte *c,int n)
{
  int i,j; 
  for(i=0;i<n;i++)
    c[i]=multtable(a[i],b[i]);

  for(j=1;j<n;j++)
  {
    for(i=0;i<j;i++)
    {
      byte tmp=get_robprg(n); //rand();
      byte tmp2=(tmp ^ multtable(a[i],b[j])) ^ multtable(a[j],b[i]);
      c[i]^=tmp;
      c[j]^=tmp2;
    }
    for(i=0;i<j;i++)
    {
      byte tmp=get_robprg(n);
      c[j]^=c[i]^tmp;
      c[i]=tmp;
    }	
  }
}
  
void subbyte_rp_share_ilr(byte *a,int n)
{
  locality_refresh(a,n); 
  subbyte_rp_share_func(a,n,multshare_ilr);
}

// multshare with internal locality refresh, improved version
void multshare_ilr2(byte *a,byte *b,byte *c,int n)
{
  int i,j; 
  for(i=0;i<n;i++)
    c[i]=multtable(a[i],b[i]);

  for(j=1;j<n;j++)
  {
    for(i=0;i<j;i++)
    {
      byte tmp=get_robprg(n); //rand();
      c[j]^=c[i] ^ tmp;
      c[i]=(tmp ^ multtable(a[i],b[j])) ^ multtable(a[j],b[i]);
    }
  }
  locality_refresh(c,n);
}

void subbyte_rp_share_ilr2(byte *a,int n)
{
  locality_refresh(a,n); 
  subbyte_rp_share_func(a,n,multshare_ilr2);
}

void multshare_flr_mprg(byte *a,byte *b,byte *c,int n)
{
  int i,j;
  
  for(i=0;i<n;i++)
    c[i]=multtable(a[i],b[i]);

  int co=0;
  for(i=0;i<n;i++)
  {
    for(j=i+1;j<n;j++)
    {
      byte tmp=get_mprgi(co);
      co++;
      byte tmp2=(tmp ^ multtable(a[i],b[j])) ^ multtable(a[j],b[i]);
      c[i]^=tmp;
      c[j]^=tmp2;
    }
  }
  locality_refresh_mprg(c,n);
}

void subbyte_rp_share_flr_mprg(byte *a,int n)
{
  locality_refresh_mprg(a,n);
  subbyte_rp_share_func(a,n,multshare_flr_mprg);
}



void multshare_ilr_mprg(byte *a,byte *b,byte *c,int n)
{
  int i,j; 
  for(i=0;i<n;i++)
    c[i]=multtable(a[i],b[i]);

  for(j=1;j<n;j++)
  {
    for(i=0;i<j;i++)
    {
      byte tmp=get_mprgi(i);
      byte tmp2=(tmp ^ multtable(a[i],b[j])) ^ multtable(a[j],b[i]);
      c[i]^=tmp;
      c[j]^=tmp2;
    }
    for(i=0;i<j;i++)
    {
      byte tmp=get_mprgc(i);
      c[j]^=c[i]^tmp;
      c[i]=tmp;
    }	
  }
}

void subbyte_rp_share_ilr_mprg(byte *a,int n)
{
  locality_refresh_mprg(a,n);
  subbyte_rp_share_func(a,n,multshare_ilr_mprg);
}

void multshare_flr_mprgmat(byte *a,byte *b,byte *c,int n)
{
  locality_refresh_mprgmatabc(a,PRGA,n);
  locality_refresh_mprgmatabc(b,PRGB,n);

  for(int i=0;i<n;i++)
    c[i]=multtable(a[i],b[i]);

  int co=0;
  for(int i=0;i<n;i++)
  {
    for(int j=i+1;j<n;j++)
    {
      byte tmp=get_mprgmati(co);
      co++;
      byte tmp2=(tmp ^ multtable(a[i],b[j])) ^ multtable(a[j],b[i]);
      c[i]^=tmp;
      c[j]^=tmp2;
    }
  }
  locality_refresh_mprgmatabc(c,PRGC,n);
}

void subbyte_rp_share_flr_mprgmat(byte *a,int n)
{
  subbyte_rp_share_func(a,n,multshare_flr_mprgmat);
}
