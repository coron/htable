// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License version 2 as published
// by the Free Software Foundation.

#include "des.h"
#include "common.h"

#include <string.h>
#include <time.h>
#include <stdio.h>

byte sbox[512]={
14,0,4,15,13,7,1,4,2,14,15,2,11,13,8,1,3,10,10,6,6,12,12,11,5,9,9,5,0,3,7,8,4,15,1,12,14,8,8,2,13,4,6,9,2,1,11,7,15,5,12,11,9,3,7,14,3,10,10,0,5,6,0,13,
15,3,1,13,8,4,14,7,6,15,11,2,3,8,4,14,9,12,7,0,2,1,13,10,12,6,0,9,5,11,10,5,0,13,14,8,7,10,11,1,10,3,4,15,13,4,1,2,5,11,8,6,12,7,6,12,9,0,3,5,2,14,15,9,
10,13,0,7,9,0,14,9,6,3,3,4,15,6,5,10,1,2,13,8,12,5,7,14,11,12,4,11,2,15,8,1,13,1,6,10,4,13,9,0,8,6,15,9,3,8,0,7,11,4,1,15,2,14,12,3,5,11,10,5,14,2,7,12,
7,13,13,8,14,11,3,5,0,6,6,15,9,0,10,3,1,4,2,7,8,2,5,12,11,1,12,10,4,14,15,9,10,3,6,15,9,0,0,6,12,10,11,1,7,13,13,8,15,9,1,4,3,5,14,11,5,12,2,7,8,2,4,14,
2,14,12,11,4,2,1,12,7,4,10,7,11,13,6,1,8,5,5,0,3,15,15,10,13,3,0,9,14,8,9,6,4,11,2,8,1,12,11,7,10,1,13,14,7,2,8,13,15,6,9,15,12,0,5,9,6,10,3,4,0,5,14,3,
12,10,1,15,10,4,15,2,9,7,2,12,6,9,8,5,0,6,13,1,3,13,4,14,14,0,7,11,5,3,11,8,9,4,14,3,15,2,5,12,2,9,8,5,12,15,3,10,7,11,0,14,4,1,10,7,1,6,13,0,11,8,6,13,
4,13,11,0,2,11,14,7,15,4,0,9,8,1,13,10,3,14,12,3,9,5,7,12,5,2,10,15,6,8,1,6,1,6,4,11,11,13,13,8,12,1,3,4,7,10,14,7,10,9,15,5,6,0,8,15,0,14,5,2,9,3,2,12,
13,1,2,15,8,13,4,8,6,10,15,3,11,7,1,4,10,12,9,5,3,6,14,11,5,0,0,14,12,9,7,2,7,2,11,1,4,14,1,7,9,4,12,10,14,8,2,13,0,15,6,12,10,9,13,0,15,3,3,5,5,6,8,11
};

int ipperm[64]={58,50,42,34,26,18,10,2,
	    60,52,44,36,28,20,12,4,
	    62,54,46,38,30,22,14,6,
	    64,56,48,40,32,24,16,8,
	    57,49,41,33,25,17,9,1,
	    59,51,43,35,27,19,11,3,
	    61,53,45,37,29,21,13,5,
	    63,55,47,39,31,23,15,7};

int ipinvperm[64]={40,8,48,16,56,24,64,32,
		   39,7,47,15,55,23,63,31,
		   38,6,46,14,54,22,62,30,
		   37,5,45,13,53,21,61,29,
		   36,4,44,12,52,20,60,28,
		   35,3,43,11,51,19,59,27,
		   34,2,42,10,50,18,58,26,
		   33,1,41,9,49,17,57,25};

int eperm[64]={32,1,2,3,4,5,0,0,
	       4,5,6,7,8,9,0,0,
	       8,9,10,11,12,13,0,0,
	       12,13,14,15,16,17,0,0,
	       16,17,18,19,20,21,0,0,
	       20,21,22,23,24,25,0,0,
	       24,25,26,27,28,29,0,0,
	       28,29,30,31,32,1,0,0};

int pperm[32]={16,7,20,21,29,12,28,17,1,15,23,26,5,18,31,10,
	       2,8,24,14,32,27,3,9,19,13,30,6,22,11,4,25};


int pc1perm[64]={57,49,41,33,25,17,9,
		 1,58,50,42,34,26,18,
		 10,2,59,51,43,35,27,
		 19,11,3,60,52,44,36,0,0,0,0,
		 63,55,47,39,31,23,15,
		 7,62,54,46,38,30,22,
		 14,6,61,53,45,37,29,
		 21,13,5,28,20,12,4,0,0,0,0};

int pc2perm[64]={14,17,11,24,1,5,0,0,
		 3,28,15,6,21,10,0,0,
		 23,19,12,4,26,8,0,0,
		 16,7,27,20,13,2,0,0,
		 45,56,35,41,51,59,0,0,
		 34,44,55,49,37,52,0,0,
		 48,53,43,60,38,57,0,0,
		 50,46,54,40,33,36,0,0};

void print_data(byte *data,int n)
{
  int i,j;
  for(i=0;i<n;i++)
  {
    printf("%02x ",data[i]);
  }
  printf("\n");
}

// select bit i of x
byte bit(byte x,int i)
{
  return (x >> (i ^ 7)) & 1;
}

// multiple bit selection function
void perm(byte *in,byte *out,int *dperm,int n) 
{
  int nb=n/8;
  int i,j,sel;
  for(i=0;i<nb;i++)
  {
    out[i]=0;
    for(j=0;j<8;j++)
    {
      sel=dperm[i*8+j]-1;
      out[i]=out[i] << 1;
      if (sel!=(-1))
	out[i]|= bit(in[sel/8],sel & 7);
    }
  }
}

void xorbyte(byte *data,byte *x,int nb)
{
  int i;
  for(i=0;i<nb;i++)
    data[i]^=x[i];
}

void fsbox(byte data[8])
{
  byte temp[8];
  int i;
  for(i=0;i<8;i++)
    temp[i]=sbox[(i<< 6) | (data[i] >> 2)];

  for(i=0;i<4;i++)
    data[i]=temp[i*2+1] | (temp[i*2] << 4);
}

void f(byte *in,byte *out,byte *ki)
{
  byte temp[8];
  perm(in,temp,eperm,64);
  xorbyte(temp,ki,8);
  fsbox(temp); 
  perm(temp,out,pperm,32);
}

void fround(byte *l,byte *r,byte *ki)
{
  byte temp[4];
  f(r,temp,ki);
  xorbyte(temp,l,4);
  memcpy(l,r,4);
  memcpy(r,temp,4);
}

// c et d are 28 bits each, bot encoded in 4 bytes
void leftshift(byte cd[8],int n)
{
  byte t;
  int i,j,k;
  for(i=0;i<n;i++)
  {
    for(k=0;k<2;k++)
    {
      t=cd[k*4] >> 7;
      for(j=0;j<3;j++)
	cd[k*4+j]=(cd[k*4+j] << 1) | (cd[k*4+j+1] >> 7);
      cd[k*4+3]=(cd[k*4+3] << 1) | (t << 4);
    }
  }
}

void gen_cd(byte key[8],byte cd[8])
{
  perm(key,cd,pc1perm,64);
}

void gen_ki(byte cd[8],byte *ki,int i)
{
  if((i==1) || (i==2) || (i==9) || (i==16))
    leftshift(cd,1);
  else leftshift(cd,2);
  perm(cd,ki,pc2perm,64);
}

void keyexpansion(byte *w,byte *key)
{
  byte cd[8];
  int i;

  gen_cd(key,cd);
  
  for(i=1;i<=16;i++)
    gen_ki(cd,w+8*(i-1),i);
}

// DES encryption with subkeys in w
void des_encrypt(byte *in,byte *out,byte *w)
{
  int i;
  byte temp[8],l[4],r[4];
 
  perm(in,temp,ipperm,64);
  
  memcpy(l,temp,4);
  memcpy(r,temp+4,4);

  for(i=1;i<=16;i++)
    fround(l,r,w+(i-1)*8);

  memcpy(temp,r,4);
  memcpy(temp+4,l,4);
  perm(temp,out,ipinvperm,64);
}

int run_des(byte *in,byte *out,byte *key,int nt)
{
  byte w[128];
  clock_t start,end;
  int i;
  
  keyexpansion(w,key);

  start=clock();
  for(i=0;i<nt;i++)
    des_encrypt(in,out,w);
  end=clock();

  return (int) (end-start);
}
