// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License version 2 as published
// by the Free Software Foundation.

#include "des_htable.h"
#include "des.h"

#include "share.h"

#include <string.h>

#define K 64

typedef unsigned int word;

void evalSbox(byte *a,byte *S,int n)
{
  unsigned char T[K][n];
  unsigned char Tp[K][n];
  byte b[n];
  int i,j,k,k2;
 
  for(k=0;k<K;k++)
    share(S[k],T[k],n);

  for(i=0;i<(n-1);i++)
  {
    for(k=0;k<K;k++)
    {
      k2=k ^ a[i];
      memcpy(Tp[k],T[k2],n);
    }

    for(k=0;k<K;k++)
    {
      memcpy(T[k],Tp[k],n);
      refresh(T[k],n);
    }
  }
  
  for(j=0;j<n;j++)
    b[j]=T[a[n-1]][j];

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

void sbox_htable_word(int ii,byte *a,int n)
{
  int w=2*sizeof(word); // number of nibbles to store in a word
  word T[n][K/w];  // n*256 bytes
  word Tp[n][K/w]; // n*256 bytes
  int i,k,k2,j;     // 16 bytes
  word r;           // 4 bytes
  word b[n];        // 4*n bytes (for 32-bit registers)
  byte x;           // 1 byte
  byte c[n];        // n bytes

  // Memory: 133*x+21

  byte *S=sbox+ii*64;

  for(k=0;k<K/w;k++)
  {
    r=0;
    for(j=w-1;j>=0;j--)
    {
      r=r << 4;
      r^=S[k*w+j];
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
    b[j]=T[j][(a[n-1] & 63)/w];

  refreshword(b,n);

  byte Ts[n][w];
  byte Tsp[n][w];

  for(k=0;k<w;k++)
    for(i=0;i<n;i++)
      Ts[i][k]=(b[i] >> (k*4)) & 15;
  
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

void sbox_htable_word_inc(int ii,byte *a,int n)
{
  int w=2*sizeof(word); // number of nibbles to store in a word
  word T[n][K/w];  // n*256 bytes
  word Tp[n][K/w]; // n*256 bytes
  int i,k,k2,j;     // 16 bytes
  word r;           // 4 bytes
  word b[n];        // 4*n bytes (for 32-bit registers)
  byte x;           // 1 byte
  byte c[n];        // n bytes

  // Memory: 133*x+21

  byte *S=sbox+ii*64;

  for(k=0;k<K/w;k++)
  {
    r=0;
    for(j=w-1;j>=0;j--)
    {
      r=r << 4;
      r^=S[k*w+j];
    }
    T[0][k]=r;
  }

  for(i=0;i<(n-1);i++)
  {
    k2=a[i]/w;

    for(j=0;j<(i+1);j++)
      for(k=0;k<K/w;k++)
	Tp[j][k]=T[j][k ^ k2];

    for(k=0;k<K/w;k++)
      T[i+1][k]=0;

    for(j=0;j<(i+1);j++)
      for(k=0;k<K/w;k++)
      {
	word tmp=xorshf96();
	T[j][k]=Tp[j][k] ^ tmp;
	T[i+1][k]=T[i+1][k] ^ tmp;
      }
  }

  for(j=0;j<n;j++)
    b[j]=T[j][(a[n-1] & 63)/w];

  refreshword(b,n);

  byte Ts[n][w];
  byte Tsp[n][w];

  for(k=0;k<w;k++)
    for(i=0;i<n;i++)
      Ts[i][k]=(b[i] >> (k*4)) & 15;
  
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
