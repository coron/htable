// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License version 2 as published
// by the Free Software Foundation.

#include "aes_htable.h"
#include "share.h"

#include <string.h>

#define K 256

typedef unsigned int word;

void subbyte_htable(byte *a,int n)
{
  byte T[n][K],Tp[n][K],b[n];
  int i,j,k;
 
  for(k=0;k<K;k++)
    T[0][k]=sbox[k];

  for(j=1;j<n;j++)
    for(k=0;k<K;k++)
      T[j][k]=0;

  
  for(i=0;i<(n-1);i++)
  {
    for(j=0;j<n;j++)
      for(k=0;k<K;k++)
	Tp[j][k]=T[j][k ^ a[i]];

    for(k=0;k<K;k++)
      T[0][k]=Tp[0][k];

    for(j=1;j<n;j++)
    {
      for(k=0;k<K;k++)
      {
	byte tmp=xorshf96();
	T[j][k]=Tp[j][k] ^ tmp;
	T[0][k]=T[0][k] ^ tmp;
      }
    }
  }

  for(j=0;j<n;j++)
    b[j]=T[j][a[n-1]];

  refresh(b,n);
  for(j=0;j<n;j++) a[j]=b[j];
}

void subbyte_htable_inc(byte *a,int n)
{
  byte T[n][K];
  byte Tp[n][K];
  byte b[n];
  int i,j,k;
 
  for(k=0;k<K;k++)
    T[0][k]=sbox[k];

  for(i=0;i<(n-1);i++)
  {
    for(k=0;k<K;k++)
      for(j=0;j<(i+1);j++)
	Tp[j][k]=T[j][k ^ a[i]];

    for(k=0;k<K;k++)
      T[i+1][k]=0;

    for(j=0;j<i+1;j++)
    {
      for(k=0;k<K;k++)
      {
	byte tmp=xorshf96();
	T[j][k]=Tp[j][k] ^ tmp;
	T[i+1][k]=T[i+1][k] ^ tmp;
      }
    }
  }
  
  for(j=0;j<n;j++)
    b[j]=T[j][a[n-1]];

  refresh(b,n);
  for(j=0;j<n;j++) a[j]=b[j];
}


void refreshword(word a[],int n)
{
  int i;
  word tmp;
  for(i=1;i<n;i++)
  {
    tmp=xorshf96(); //rand();
    a[0]=a[0] ^ tmp;
    a[i]=a[i] ^ tmp;
  }
}

void subbyte_htable_word(byte *a,int n)  // n+4 bytes
{
  int w=sizeof(word); // number of bytes to store in a word w=4
  word T[n][K/w];  // n*256 bytes
  word Tp[n][K/w]; // n*256 bytes
  int i,k,k2,j;    // 16 bytes
  word r;          // 4 bytes
  word b[n];       // 4*n bytes (for 32-bit registers)
  byte x;          // 1 bytes
  byte c[n];       // n bytes

  // Memory:  518*n+25

  for(k=0;k<K/w;k++)
  {
    r=0;
    for(j=w-1;j>=0;j--)
    {
      r=r << 8;
      r^=sbox[k*w+j];
    }
    T[0][k]=r;
  }

  for(i=1;i<n;i++)
    for(k=0;k<K/w;k++)
      T[i][k]=0;


  for(i=0;i<(n-1);i++)
  {
    k2=a[i]/w;
    for(j=0;j<n;j++)
      for(k=0;k<K/w;k++)
	Tp[j][k]=T[j][k ^ k2];

    for(k=0;k<K/w;k++)
      T[n-1][k]=Tp[n-1][k];

    for(j=0;j<n-1;j++)
      for(k=0;k<K/w;k++)
      {
	word tmp=xorshf96();
	T[j][k]=Tp[j][k] ^ tmp;
	T[n-1][k]=T[n-1][k] ^ tmp;
      }
  }

  for(j=0;j<n;j++)
    b[j]=T[j][a[n-1]/w];

  refreshword(b,n);
  
  // not counted above because the tables T and Tp can be discarded
  byte Ts[n][w];   // 4*n bytes
  byte Tsp[n][w];  // 4*n bytes

  for(i=0;i<n;i++)
    for(k=0;k<w;k++)
      Ts[i][k]=b[i] >> (k*8);
  
  for(i=0;i<(n-1);i++)
  {
    byte s=a[i] & (w-1);
    for(j=0;j<n;j++)
      for(k=0;k<w;k++)
	Tsp[j][k]=Ts[j][k^s];

    for(k=0;k<w;k++)
      Ts[0][k]=Tsp[0][k];

    for(j=1;j<n;j++)
    {
      for(k=0;k<w;k++)
      {
	byte tmp=xorshf96();
	Ts[j][k]=Tsp[j][k] ^ tmp;
	Ts[0][k]=Ts[0][k] ^ tmp;
      }
    }
  }

  for(j=0;j<n;j++)
    a[j]=Ts[j][a[n-1] & (w-1)];

  refresh(a,n);
}

void subbyte_htable_word_inc(byte *a,int n)  // n+4 bytes
{
  int w=sizeof(word); // number of bytes to store in a word w=4
  word T[n][K/w];  // n*256 bytes
  word Tp[n][K/w]; // n*256 bytes
  int i,k,k2,j;    // 16 bytes
  word r;          // 4 bytes
  word b[n];       // 4*n bytes (for 32-bit registers)
  byte x;          // 1 bytes
  byte c[n];       // n bytes

  for(k=0;k<K/w;k++)
  {
    r=0;
    for(j=w-1;j>=0;j--)
    {
      r=r << 8;
      r^=sbox[k*w+j];
    }
    T[0][k]=r;
  }

  for(i=0;i<(n-1);i++)
  {
    k2=a[i]/w;

    for(j=0;j<(i+1);j++)
      for(k=0;k<K;k++)
	Tp[j][k]=T[j][k ^ k2];

    for(k=0;k<K/w;k++)
      T[i+1][k]=0;

    for(j=0;j<(i+1);j++)
    {
      for(k=0;k<K/w;k++)
      {
	word tmp=xorshf96();
	T[j][k]=Tp[j][k] ^ tmp;
	T[i+1][k]=T[i+1][k] ^ tmp;
      }
    }
  }

  for(j=0;j<n;j++)
    b[j]=T[j][a[n-1]/w];

  refreshword(b,n);
  
  // not counted above because the tables T and Tp can be discarded
  byte Ts[n][w];   // 4*n bytes
  byte Tsp[n][w];  // 4*n bytes

  for(i=0;i<n;i++)
    for(k=0;k<w;k++)
      Ts[i][k]=b[i] >> (k*8);
  
  for(i=0;i<(n-1);i++)
  {
    byte s=a[i] & (w-1);
    for(j=0;j<n;j++)
      for(k=0;k<w;k++)
	Tsp[j][k]=Ts[j][k^s];

    for(k=0;k<w;k++)
      Ts[0][k]=Tsp[0][k];

    for(j=1;j<n;j++)
    {
      for(k=0;k<w;k++)
      {
	byte tmp=xorshf96();
	Ts[j][k]=Tsp[j][k] ^ tmp;
	Ts[0][k]=Ts[0][k] ^ tmp;
      }
    }
  }

  for(j=0;j<n;j++)
    a[j]=Ts[j][a[n-1] & (w-1)];

  refresh(a,n);
}
