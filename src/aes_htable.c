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
  byte T[K][n],Tp[K][n],b[n];
  int i,j,k;
 
  for(k=0;k<K;k++)
    share(sbox[k],T[k],n);

  for(i=0;i<(n-1);i++)
  {
    for(k=0;k<K;k++)
      for(j=0;j<n;j++)
	Tp[k][j]=T[k ^ a[i]][j];

    for(k=0;k<K;k++)
    {
      for(j=0;j<n;j++) 
	T[k][j]=Tp[k][j];
      refresh(T[k],n);
    }
  }
  
  for(j=0;j<n;j++)
    b[j]=T[a[n-1]][j];

  refresh(b,n);
  for(j=0;j<n;j++) a[j]=b[j];
}

void subbyte_htable_trans(byte *a,int n)
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
  byte T[K][n];
  byte Tp[K][n];
  byte b[n];
  int i,j,k;
 
  for(k=0;k<K;k++)
    share(sbox[k],T[k],1);

  for(i=0;i<(n-1);i++)
  {
    for(k=0;k<K;k++)
      for(j=0;j<(i+1);j++)
	Tp[k][j]=T[k ^ a[i]][j];

    for(k=0;k<K;k++)
    {
      for(j=0;j<(i+1);j++) 
	T[k][j]=Tp[k][j];
      T[k][i+1]=0;
      refresh(T[k],i+2);
    }
  }
  
  for(j=0;j<n;j++)
    b[j]=T[a[n-1]][j];

  refresh(b,n);
  for(j=0;j<n;j++) a[j]=b[j];
}

void subbyte_htable_inc_trans(byte *a,int n)
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
  word T[K/w][n];  // n*256 bytes
  word Tp[K/w][n]; // n*256 bytes
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
    T[k][0]=r;
    for(i=1;i<n;i++)
      T[k][i]=0;
  }

  for(i=0;i<(n-1);i++)
  {
    k2=a[i]/w;
    for(k=0;k<K/w;k++)
      memcpy(Tp[k],T[k ^ k2],n*sizeof(word));
    for(k=0;k<K/w;k++)
    {
      memcpy(T[k],Tp[k],n*sizeof(word));
      refreshword(T[k],n);
    }
  }

  memcpy(b,T[a[n-1]/w],n*sizeof(word));
  refreshword(b,n);
  
  // not counted above because the tables T and Tp can be discarded
  byte Ts[w][n];   // 4*n bytes
  byte Tsp[w][n];  // 4*n bytes

  for(k=0;k<w;k++)
    for(i=0;i<n;i++)
      Ts[k][i]=b[i] >> (k*8);
  
  for(i=0;i<(n-1);i++)
  {
    for(k=0;k<w;k++)
      memcpy(Tsp[k],Ts[k ^ (a[i] & (w-1))],n);
    for(k=0;k<w;k++)
    {
      memcpy(Ts[k],Tsp[k],n);
      refresh(Ts[k],n);
    }
  }
  memcpy(a,Ts[a[n-1] & (w-1)],n);
  refresh(a,n);
}

void subbyte_htable_word_inc(byte *a,int n)  // n+4 bytes
{
  int w=sizeof(word); // number of bytes to store in a word w=4
  word T[K/w][n];  // n*256 bytes
  word Tp[K/w][n]; // n*256 bytes
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
    T[k][0]=r;
  }

  for(i=0;i<(n-1);i++)
  {
    k2=a[i]/w;
    for(k=0;k<K/w;k++)
      memcpy(Tp[k],T[k ^ k2],(i+1)*sizeof(word));
    for(k=0;k<K/w;k++)
    {
      memcpy(T[k],Tp[k],(i+1)*sizeof(word));
      T[k][i+1]=0;
      refreshword(T[k],i+2);
    }
  }

  memcpy(b,T[a[n-1]/w],n*sizeof(word));
  refreshword(b,n);
  
  // not counted above because the tables T and Tp can be discarded
  byte Ts[w][n];   // 4*n bytes
  byte Tsp[w][n];  // 4*n bytes

  for(k=0;k<w;k++)
    for(i=0;i<n;i++)
      Ts[k][i]=b[i] >> (k*8);
  
  for(i=0;i<(n-1);i++)
  {
    for(k=0;k<w;k++)
      memcpy(Tsp[k],Ts[k ^ (a[i] & (w-1))],n);
    for(k=0;k<w;k++)
    {
      memcpy(Ts[k],Tsp[k],n);
      refresh(Ts[k],n);
    }
  }
  memcpy(a,Ts[a[n-1] & (w-1)],n);
  refresh(a,n);
}

void subbyte_htable_word_inc_trans(byte *a,int n)  // n+4 bytes
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
