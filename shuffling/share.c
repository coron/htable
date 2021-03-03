// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License version 2 as published
// by the Free Software Foundation.

#include "share.h"
#include "aes.h"
#include <assert.h>
#include <string.h>

static unsigned long x=122456789, y=362436069, z=521288629;
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

void init_randcount()
{
  randcount=0;
}

unsigned int get_randcount()
{
  return randcount;
}

void set_randcount(unsigned int randc)
{
  randcount=randc;
}

void share(byte x,byte a[],int n)
{
  int i;
  a[0]=x;
  for(i=1;i<n;i++)
    a[i]=0;
}

// This is the t-NI refresh
void refresh(byte a[],int n)
{
  int i;
  for(i=1;i<n;i++)
  {
    byte tmp=xorshf96(); //rand();
    a[0]=a[0] ^ tmp;
    a[i]=a[i] ^ tmp;
  }
}

void encode(byte x,byte *a,int n)
{
  share(x,a,n);
  refresh(a,n);
}

byte decode(byte a[],int n)
{
  byte r=0;
  for(int i=0;i<n;i++)
    r^=a[i];
  return r;
}

// We must have dest!=in
void rotate_shuffling(byte *in,byte *dest,int delta,int ell)
{
  //for(int i=0;i<ell;i++)
  //  dest[i]=in[(ell+i-delta) % ell];

  byte temp[2*ell];
  memcpy(temp,in,ell);
  memcpy(temp+ell,in,ell);
  memcpy(dest,temp+ell-delta,ell);
}

void reshuffle(byte a[],int *ind,int ell)
{
  int delta=xorshf96() % ell;
  *ind=(*ind+delta) % ell;  
  byte temp[ell];
  rotate_shuffling(a,temp,delta,ell);
  for(int i=0;i<ell;i++)
    a[i]=temp[i];

}

void reshuffle_s(byte *a,int *ind,int k,int ell)
{
  for(int i=0;i<k;i++)
    reshuffle(a+i*ell,ind+i,ell);
}

void encode_s(byte x,byte *a,int *ind,int k,int ell)
{
  for(int i=0;i<k*ell;i++) a[i]=0;
  for(int i=0;i<k;i++) ind[i]=0;

  byte temp[k];

  encode(x,temp,k);
  for(int i=0;i<k;i++)
    a[i*ell]=temp[i];

  reshuffle_s(a,ind,k,ell);
}

byte decode_s(byte *a,int *ind,int k,int ell)
{
  byte r=0;
  for(int i=0;i<k;i++)
    r^=a[i*ell+ind[i]];
  return r;
}

void xor_gate_shuffling(byte *a,int ia,byte *b,int ib,byte *c,int *ic,int ell)
{
  *ic=xorshf96() % ell;
  byte ta[ell];
  rotate_shuffling(a,ta,*ic-ia,ell);
  byte tb[ell];
  rotate_shuffling(b,tb,*ic-ib,ell);
  for(int i=0;i<ell;i++)
    c[i]=ta[i] ^ tb[i];
}

void xor_gate_shuffling_noind(byte *a,byte *b,byte *c,int ell)
{
  for(int i=0;i<ell;i++)
    c[i]=a[i] ^ b[i];
}
  
void test_xor_gate_shuffling(int ell)
{
  byte a[ell],b[ell],c[ell];
  byte va=45,vb=34;
  int ia=0,ib=0,ic;

  share(va,a,ell);
  reshuffle(a,&ia,ell);
  
  share(vb,b,ell);
  reshuffle(b,&ib,ell);
  
  xor_gate_shuffling(a,ia,b,ib,c,&ic,ell);

  assert(c[ic]==(va ^ vb));
}


void xor_gate_s(byte *a,int *ia,byte *b,int *ib,byte *c,int *ic,int k,int ell)
{
  for(int i=0;i<k;i++)
    xor_gate_shuffling(a+i*ell,ia[i],b+i*ell,ib[i],c+i*ell,ic+i,ell);
}

void test_xor_gate_s(int k,int ell)
{
  byte a[ell*k],b[ell*k],c[ell*k];
  int inda[k],indb[k],indc[k];

  byte va=10,vb=45;
  encode_s(va,a,inda,k,ell);
  encode_s(vb,b,indb,k,ell);
  
  xor_gate_s(a,inda,b,indb,c,indc,k,ell);

  assert(decode_s(c,indc,k,ell) == (va ^ vb));
}

void multx_s(byte *a,int *ia,byte *b,int *ib,int k,int ell)
{
  for(int i=0;i<k*ell;i++)
    b[i]=multx(a[i]);
  memcpy(ib,ia,k*sizeof(int));
}

void mult3_s(byte *a,int *ia,byte *b,int *ib,int k,int ell)
{
  for(int i=0;i<k*ell;i++)
    b[i]=mult3(a[i]);
  memcpy(ib,ia,k*sizeof(int));
}



/*
void refresh_s(byte a[],int k,int ell)
{
  int i;
  for(int j=0;j<ell;j++)
  {
    for(int i=0;i<k;i++)
    {
      byte tmp=xorshf96(); //rand();
      a[0*ell+j]^= tmp;
      a[i*ell+j]^= tmp;
    }
  }
}

*/

  







