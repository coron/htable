#include "share.h"

static uint64_t x=123456789, y=362436069, z=521288629;

uint64_t xorshf96(void) {   
  uint64_t t;
  x ^= x << 16;
  x ^= x >> 5;
  x ^= x << 1;

  t = x;
  x = y;
  y = z;
  z = t ^ x ^ y;
  return z;
}

void refresh(byte a[],uint8_t n)
{
  uint8_t i;
  for(i=1;i<n;i++)
  {
    byte tmp=(byte)xorshf96(); //rand();
    a[0]=a[0] ^ tmp;
    a[i]=a[i] ^ tmp;
  }
}

void share(byte x,byte a[],uint8_t n)
{
  uint8_t i;
  a[0]=x;
  for(i=1;i<n;i++)
    a[i]=0;
}

byte xorop(byte a[],uint8_t n)
{
  uint8_t i;
  byte r=0;
  for(i=0;i<n;i++)
    r^=a[i];
  return r;
}

byte decode(byte a[],uint8_t n)
{
  uint8_t i;
  for(i=0;i<n;i++)
    refresh(a,n);
  return xorop(a,n);
}

