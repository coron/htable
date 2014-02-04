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
  word T[K/w][n];   // n*64 bytes
  word Tp[K/w][n];  // n*64 bytes
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
    T[k][0]=r;
    for(i=1;i<n;i++)
      T[k][i]=0;
  }

  for(i=0;i<(n-1);i++)
  {
    k2=(a[i] & 63)/w;
    for(k=0;k<K/w;k++)
      memcpy(Tp[k],T[k ^ k2],n*sizeof(word));
    for(k=0;k<K/w;k++)
    {
      memcpy(T[k],Tp[k],n*sizeof(word));
      refreshword(T[k],n);
    }
  }

  memcpy(b,T[(a[n-1] & 63)/w],n*sizeof(word));

  byte Ts[w][n];
  byte Tsp[w][n];

  for(k=0;k<w;k++)
    for(i=0;i<n;i++)
      Ts[k][i]=(b[i] >> (k*4)) & 15;
  
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
