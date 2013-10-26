#include "share.h"

static unsigned long x=123456789, y=362436069, z=521288629;

unsigned long xorshf96(void) {   
  unsigned long t;
  x ^= x << 16;
  x ^= x >> 5;
  x ^= x << 1;

  t = x;
  x = y;
  y = z;
  z = t ^ x ^ y; 
  return z;
}

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

void share(byte x,byte a[],int n)
{
  int i;
  a[0]=x;
  for(i=1;i<n;i++)
    a[i]=0;
}

byte xorop(byte a[],int n)
{
  int i;
  byte r=0;
  for(i=0;i<n;i++)
    r^=a[i];
  return r;
}

byte decode(byte a[],int n)
{
  int i;
  for(i=0;i<n;i++)
    refresh(a,n);
  return xorop(a,n);
}

