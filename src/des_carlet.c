// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License version 2 as published
// by the Free Software Foundation.


#include "des_carlet.h"
#include "des.h"
#include "share.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int nmult;

byte logt[64]={
0x00,0x3f,0x01,0x38,0x02,0x31,0x39,0x14,
0x03,0x0d,0x32,0x35,0x3a,0x19,0x15,0x2a,
0x04,0x23,0x0e,0x10,0x33,0x28,0x36,0x12,
0x3b,0x1f,0x1a,0x06,0x16,0x2e,0x2b,0x25,
0x05,0x1e,0x24,0x2d,0x0f,0x22,0x11,0x27,
0x34,0x0c,0x29,0x18,0x37,0x3e,0x13,0x30,
0x3c,0x0a,0x20,0x1c,0x1b,0x09,0x07,0x08,
0x17,0x0b,0x2f,0x3d,0x2c,0x1d,0x26,0x21};

byte alogt[64]={
0x01,0x02,0x04,0x08,0x10,0x20,0x1b,0x36,
0x37,0x35,0x31,0x39,0x29,0x09,0x12,0x24,
0x13,0x26,0x17,0x2e,0x07,0x0e,0x1c,0x38,
0x2b,0x0d,0x1a,0x34,0x33,0x3d,0x21,0x19,
0x32,0x3f,0x25,0x11,0x22,0x1f,0x3e,0x27,
0x15,0x2a,0x0f,0x1e,0x3c,0x23,0x1d,0x3a,
0x2f,0x05,0x0a,0x14,0x28,0x0b,0x16,0x2c,
0x03,0x06,0x0c,0x18,0x30,0x3b,0x2d,0x01};

byte sq[64]={
0x00,0x01,0x04,0x05,0x10,0x11,0x14,0x15,
0x1b,0x1a,0x1f,0x1e,0x0b,0x0a,0x0f,0x0e,
0x37,0x36,0x33,0x32,0x27,0x26,0x23,0x22,
0x2c,0x2d,0x28,0x29,0x3c,0x3d,0x38,0x39,
0x31,0x30,0x35,0x34,0x21,0x20,0x25,0x24,
0x2a,0x2b,0x2e,0x2f,0x3a,0x3b,0x3e,0x3f,
0x06,0x07,0x02,0x03,0x16,0x17,0x12,0x13,
0x1d,0x1c,0x19,0x18,0x0d,0x0c,0x09,0x08};

// Lagrange Interpolation of the DES Sboxes
byte sl[504]={14,32,8,4,10,36,32,5,52,28,29,26,1,35,34,7,1,63,5,53,20,19,39,62,28,3,54,36,17,45,31,15,16,59,42,49,21,5,54,47,57,0,31,14,45,39,42,52,13,13,16,33,24,11,12,57,8,17,63,46,0,19,57,
15,25,41,60,43,13,61,44,34,42,0,13,45,26,0,31,48,8,7,10,16,33,31,48,6,0,28,54,25,37,29,24,7,1,35,39,37,17,21,61,52,3,39,52,54,35,31,51,47,61,52,38,20,51,25,1,47,46,10,12,55,31,55,
10,59,50,30,16,8,1,57,17,11,10,0,2,62,43,51,19,54,43,27,23,60,46,54,1,17,1,50,33,42,63,58,31,11,51,0,41,54,33,42,25,21,56,62,57,16,37,29,25,38,62,39,21,55,40,39,31,20,57,51,20,41,16,
7,32,32,16,9,10,30,55,61,11,62,18,53,16,5,57,45,55,8,48,48,56,43,32,32,33,52,12,26,15,11,34,24,5,13,36,15,0,27,40,63,47,50,28,40,9,36,39,20,12,63,36,47,46,5,53,6,46,54,6,31,23,33,
2,25,32,61,52,2,24,35,4,22,28,43,30,40,57,13,45,18,45,55,41,47,10,27,9,52,37,46,6,55,47,23,37,8,23,39,50,61,19,13,55,21,40,25,41,20,36,43,54,3,1,21,59,14,59,5,48,11,25,33,57,41,59,
12,34,12,3,7,7,1,43,22,27,17,12,53,49,7,47,49,62,25,25,31,23,55,33,51,26,13,1,63,37,21,20,12,62,38,9,1,5,15,44,22,41,27,21,40,27,22,14,59,50,4,38,11,57,22,55,32,28,53,7,37,18,51,
4,45,31,0,5,31,21,59,42,12,41,0,63,41,53,38,13,50,4,63,52,52,13,33,61,0,14,51,53,17,62,23,23,33,15,7,2,27,5,10,51,6,62,34,59,63,44,43,56,11,32,43,26,15,12,44,54,51,52,42,10,4,58,
	      13,32,32,18,32,17,12,14,31,38,24,38,63,37,9,52,26,50,7,37,27,28,45,23,39,52,63,43,59,28,49,23,41,37,36,23,32,29,22,11,1,25,31,39,51,26,17,39,44,37,30,44,58,15,63,29,13,11,50,43,35,49,61};

// The Roy-Vivek polynomials for DES Sboxes
byte Roy[560]={13, 26, 53, 18, 47, 47, 30, 46, 49, 45, 15, 45, 3, 7, 27, 57, 40, 13, 21, 15, 12, 57, 8, 17, 63, 46, 0, 19, 57, 0, 10, 8, 48, 13, 54, 63, 45, 30, 0, 1, 21, 63, 19, 45, 17, 17, 35, 10, 15, 0, 3, 31, 45, 29, 15, 0, 20, 58, 0, 1, 53, 25, 50, 61, 6, 34, 27, 47, 10, 1, 0, 58, 11, 21, 45, 52, 49, 39, 47, 25, 47, 3, 59, 61, 56, 10, 58, 35, 27, 7, 25, 1, 47, 46, 10, 12, 55, 31, 55, 0, 54, 21, 20, 12, 58, 24, 41, 23, 0, 1, 45, 41, 38, 26, 43, 39, 2, 13, 13, 0, 18, 42, 42, 2, 16, 14, 61, 41, 0, 1, 6, 1, 43, 48, 54, 62, 19, 39, 23, 1, 29, 7, 3, 45, 23, 15, 27, 55, 43, 0, 24, 50, 56, 57, 33, 38, 20, 62, 11, 16, 40, 39, 31, 20, 57, 51, 20, 41, 16, 0, 6, 41, 0, 39, 60, 54, 60, 7, 0, 1, 46, 52, 10, 19, 26, 0, 12, 28, 9, 0, 22, 30, 27, 25, 54, 43, 55, 60, 0, 1, 55, 56, 62, 37, 16, 12, 2, 52, 14, 1, 63, 26, 63, 48, 22, 19, 42, 52, 46, 47, 11, 6, 16, 51, 28, 22, 33, 31, 2, 40, 5, 53, 6, 46, 54, 6, 31, 23, 33, 0, 40, 51, 34, 18, 21, 63, 16, 2, 0, 1, 57, 62, 56, 59, 9, 36, 46, 0, 57, 0, 2, 8, 46, 53, 21, 59, 13, 18, 0, 1, 7, 53, 14, 47, 12, 24, 12, 33, 11, 1, 50, 12, 13, 33, 16, 2, 42, 8, 43, 42, 42, 53, 9, 1, 61, 47, 21, 43, 30, 56, 59, 5, 48, 11, 25, 33, 57, 41, 59, 0, 21, 63, 24, 25, 47, 54, 17, 25, 0, 1, 39, 22, 16, 45, 0, 29, 0, 30, 9, 0, 6, 11, 5, 35, 4, 29, 17, 11, 0, 1, 63, 4, 40, 39, 29, 8, 32, 28, 0, 1, 35, 58, 46, 60, 50, 45, 16, 30, 13, 47, 15, 28, 49, 4, 19, 44, 38, 46, 63, 29, 22, 55, 32, 28, 53, 7, 37, 18, 51, 0, 62, 6, 22, 14, 29, 47, 32, 63, 0, 1, 20, 4, 0, 58, 10, 44, 34, 21, 1, 0, 6, 29, 58, 10, 20, 9, 24, 47, 0, 1, 12, 60, 8, 11, 56, 46, 25, 27, 39, 1, 51, 55, 23, 44, 11, 29, 16, 6, 26, 34, 42, 37, 57, 54, 53, 63, 40, 7, 6, 12, 12, 44, 54, 51, 52, 42, 10, 4, 58, 0, 32, 25, 43, 55, 20, 23, 47, 45, 0, 1, 16, 16, 47, 61, 41, 61, 11, 63, 16, 0, 24, 8, 23, 58, 35, 40, 33, 58, 0, 1, 56, 10, 39, 27, 43, 46, 60, 27, 32, 1, 16, 3, 40, 49, 55, 7, 19, 51, 58, 40, 40, 8, 38, 45, 31, 50, 6, 52, 17, 9, 63, 29, 13, 11, 50, 43, 35, 49, 61, 0, 60, 46, 11, 4, 22, 31, 30, 1, 0, 1, 15, 8, 33, 12, 47, 38, 33, 46, 17, 0, 0, 52, 17, 51, 14, 26, 42, 40, 0, 1, 56, 10, 62, 21, 57, 26, 34, 5, 58, 1};

// Multiplication by t in GF(2^6)
byte multx(byte x)
{
  byte y=x;
  y=y << 1;
  //if((x & 0x20)!=0)  y=y ^ 0x5b;
  byte m[]={0,0x5b};
  y^=m[x >> 5];
  return y;
}

// Slow multiplication in GF(2^6)
byte mult(byte x,byte y)
{
  int i;
  byte z=0;
  byte a=32;
  for(i=5;i>=0;i--)
  {
    z=multx(z);
    if((y & a)!=0) z^=x;
    a=a >> 1;
  }
  return z;
}


void genlogtable()
{
  int i;
  byte x=1;
  for(i=0;i<64;i++)
  {
    logt[x]=(byte) i;
    alogt[i]=x;
    x=mult(x,2);
  }

  x=0;
  printf("byte logt[64]={");
  for(i=0;i<64;i++)
  {
    if((i%8)==0) printf("\n");
    printf("0x%02x",logt[x]);
    x++;
    if(i<63) printf(",");
  }
  printf("};\n\n");

  x=0;
  printf("byte alogt[64]={");
  for(i=0;i<64;i++)
  {
    if((i%8)==0) printf("\n");
    printf("0x%02x",alogt[x]);
    x++;
    if(i<63) printf(",");
  }
  printf("};\n");
}

byte multtable(byte x,byte y)
{
  byte temp[2];
  x=x & 63;
  y=y & 63;
  int z=((x==0) || (y==0)) & 1;
  temp[0]=alogt[(logt[x]+logt[y]) % 63];
  temp[1]=0;
  return temp[z];
}

void gensquare()
{
  int i;
  byte sq[64];
  for(i=0;i<64;i++)
    sq[i]=multtable(i,i);

  printf("byte sq[64]={");
  for(i=0;i<64;i++)
  {
    if((i%8)==0) printf("\n");
    printf("0x%02x",sq[i]);
    if(i<63) printf(",");
  }
  printf("};\n");
}


byte poly(byte *pol,int d,byte x)
{
  int i;
  byte r=0;
  for(i=d;i>=0;i--)
    r=multtable(r,x) ^ pol[i];
  return r;
}

void testlagrange()
{
  int i;
  byte x;
  for(i=0;i<8;i++)
    for(x=0;x<64;x++)
      if(poly(sl+63*i,62,x)!=sbox[x+64*i])
      {
	fprintf(stderr,"Incorrect Lagrange interpolation\n");
	exit(EXIT_FAILURE);
      }
}

byte polygen(byte *pol,int d,byte x)
{
  int depth=2;
  int np=1 << depth;
  int i,j;
  byte r[np];
  byte xnp=x;
  byte y;

  for(i=0;i<depth;i++)
    xnp=multtable(xnp,xnp);

  byte z=xnp;

  for(i=0;i<np;i++)
    r[i]=pol[i];

  for(i=np;(i+np-1)<=d;i+=np)
  {
    for(j=0;j<np;j++)
      r[j]^=multtable(z,pol[i+j]);
    z=multtable(z,xnp);
  }
  for(j=0;(i+j)<=d;j++)
    r[j]^=multtable(z,pol[i+j]);
  
  y=r[np-1];
  for(j=np-2;j>=0;j--)
  {
    y=multtable(y,x);
    y^=r[j];
  }
  return y;
}

void multshare(byte *a,byte *b,byte *c,int n)
{
  int i,j;

  for(i=0;i<n;i++)
    c[i]=multtable(a[i],b[i]);

  for(i=0;i<n;i++)
  {
    for(j=i+1;j<n;j++)
    {
      byte tmp=xorshf96(); // rand();
      byte tmp2=(tmp ^ multtable(a[i],b[j])) ^ multtable(a[j],b[i]);
      c[i]^=tmp;
      c[j]^=tmp2;
    }
  }
}

void square_share(byte *a,byte *b,int n)
{
  int i;
  byte temp;
  for(i=0;i<n;i++)
    b[i]=sq[a[i] & 63];
}

// Parity-split method
void polygen_share(int ii,byte *x,int n)
{
  int d=62;
  int depth=2;
  int np=1 << depth;
  int i,j,k;
  byte *r[np];
  byte xnp[n];
  byte temp[n];
  int dp=d/np;
  nmult=0;
  byte *zi[dp+1];

  byte *pol=sl+63*ii;

  for(i=0;i<np;i++)
    r[i]=malloc(n*sizeof(byte));

  for(i=0;i<=dp;i++)
    zi[i]=malloc(n*sizeof(byte));

  memcpy(xnp,x,n);

  for(i=0;i<depth;i++)
    for(j=0;j<n;j++)
      xnp[j]=multtable(xnp[j],xnp[j]);

  memcpy(zi[1],xnp,n);

  for(i=0;i<np;i++)
    share(pol[i],r[i],n);

  int deg=1;
  for(i=np;(i+np-1)<=d;i+=np)
  {
    for(j=0;j<np;j++)
      for(k=0;k<n;k++)
	r[j][k]^=multtable(zi[deg][k],pol[i+j]);
    deg++;
    if((deg % 2)==0)
    {
      for(k=0;k<n;k++)
	zi[deg][k]=multtable(zi[deg/2][k],zi[deg/2][k]);
    }
    else
    {
      multshare(zi[deg-1],xnp,zi[deg],n);
      nmult++;
    }
  }
  
  for(j=0;(i+j)<=d;j++)
    for(k=0;k<n;k++)
      r[j][k]^=multtable(zi[deg][k],pol[i+j]);
  
  byte y[n];
  memcpy(y,r[np-1],n);
  for(j=np-2;j>=0;j--)
  {
    multshare(y,x,temp,n);
    memcpy(y,temp,n);
    for(k=0;k<n;k++)
      y[k]^=r[j][k];
    nmult++;
  }

  for(i=0;i<np;i++)
    free(r[i]);

  for(i=0;i<=dp;i++)
    free(zi[i]);
  memcpy(x,y,n);
}
    
void poly_share(byte *pol,int d,byte *x,byte *y,int n)
{
  int i;
  byte c[n];
  share(0,y,n);
  for(i=d;i>=0;i--)
  {
    multshare(y,x,c,n);
    memcpy(y,c,n);
    y[0]^=pol[i];
  }
}


byte polyRoy(byte *pol,byte x)
{
  int i;
  byte x9=1;
  for(i=0;i<9;i++)
    x9=multtable(x9,x);
  byte x18=multtable(x9,x9);
  byte x36=multtable(x18,x18);
  
  byte c=poly(pol,9,x);
  byte c1=poly(pol+10,9,x);
  byte q1=poly(pol+20,9,x);
  byte s1=poly(pol+30,9,x);
  byte c2=poly(pol+40,9,x);
  byte q2=poly(pol+50,9,x);
  byte s2=poly(pol+60,9,x);

  return multtable(x36 ^ c,multtable(x18 ^ c1,q1) ^ s1) ^ 
    multtable(x18 ^ c2,q2) ^ s2;
}

void testlagrangeRoy()
{
  int i,j;
  byte x;
  for(i=0;i<8;i++)
    for(x=0;x<64;x++)
      if(polyRoy(Roy+70*i,x)!=sbox[x+64*i])
      {
	fprintf(stderr,"Incorrect Lagrange interpolation %d %02x %02x %02x\n",
		i,x,polyRoy(Roy+70*i,x),sbox[x+64*i]);
	for(j=0;j<7;j++)
	  printf("%d\n",poly(Roy+10*j,9,0));
	exit(EXIT_FAILURE);
      }
}

void polyRoy_share(int ii,byte *x,int n)
{
  int i,j,k;
  byte zi[10][n];
  byte v[7][n];
  byte y[n];
  byte x18[n];
  byte x36[n];
  byte temp[n];
  byte temp2[n];
  byte temp3[n];

  // Memory: 23*n+3

  byte *pol=Roy+70*ii;

  memcpy(zi[1],x,n);
 
  nmult=0;
  for(i=2;i<=9;i++)
  {
    if((i % 2)==0)
      square_share(zi[i/2],zi[i],n);
    else
    {
      multshare(zi[i-1],x,zi[i],n);
      nmult++;
    }
  }
 
  square_share(zi[9],x18,n);
  square_share(x18,x36,n);

  for(i=0;i<7;i++)
  {
    share(pol[i*10],v[i],n);
    for(j=1;j<=9;j++)
      for(k=0;k<n;k++)
	v[i][k]^=multtable(pol[i*10+j],zi[j][k]);
  }

  for(k=0;k<n;k++)
  {
    v[0][k]^=x36[k];
    v[1][k]^=x18[k];
    v[4][k]^=x18[k];
  }

  
  multshare(v[1],v[2],temp,n);
  for(k=0;k<n;k++) temp[k]^=v[3][k];

  multshare(temp,v[0],temp2,n);
  
  multshare(v[4],v[5],temp3,n);

  nmult+=3;

  for(k=0;k<n;k++)
    x[k]=temp2[k] ^ temp3[k] ^ v[6][k];

}

void fsbox_carlet(byte data[8])
{
  byte temp[8];
  int i;

  for(i=0;i<8;i++)
    //temp[i]=polygen(sl+63*i,62,data[i] >> 2);
    temp[i]=polyRoy(Roy+70*i,data[i] >> 2);

  for(i=0;i<4;i++)
    data[i]=temp[i*2+1] | (temp[i*2] << 4);
}

void f_carlet(byte *in,byte *out,byte *ki)
{
  byte temp[8];
  perm(in,temp,eperm,64);
  xorbyte(temp,ki,8);
  fsbox_carlet(temp);
  perm(temp,out,pperm,32);
}

void fround_carlet(byte *l,byte *r,byte *ki)
{
  byte temp[4];
  f_carlet(r,temp,ki);
  xorbyte(temp,l,4);
  memcpy(l,r,4);
  memcpy(r,temp,4);
}

void des_encrypt_carlet(byte *in,byte *out,byte *key)
{
  int i;
  byte temp[8],l[4],r[4];
  byte w[128];

  keyexpansion(w,key);

  perm(in,temp,ipperm,64);

  memcpy(l,temp,4);
  memcpy(r,temp+4,4);

  for(i=1;i<=16;i++)
    fround_carlet(l,r,w+(i-1)*8);

  memcpy(temp,r,4);
  memcpy(temp+4,l,4);
  perm(temp,out,ipinvperm,64);
}
