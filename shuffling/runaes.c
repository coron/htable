// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License version 2 as published
// by the Free Software Foundation.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

#include "aes.h"
#include "aes_rp.h"
#include "share.h"
#include "aes_share.h"
#include "common.h"


void printMes(char *s,byte *m)
{
  printf("%s=",s);
  int i;
  for(i=0;i<16;i++) printf("%02x",m[i]);
  printf("\n");
}

void print_share_s(byte *a,int k,int ell)
{
  for(int i=0;i<k;i++)
  {
    for(int j=0;j<ell;j++)
      printf("%02x ",a[i*ell+j]);
    printf(" ");
  }
  printf("\n");
}

int main()
{
  int n;
  int nt=5;
  int i;
  byte keyex[16]={0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};

  byte inex[16]={0x32,0x43,0xf6,0xa8,0x88,0x5a,0x30,0x8d,0x31,0x31,0x98,0xa2,0xe0,0x37,0x07,0x34};

  byte outex[16]={0x39,0x25,0x84,0x1d,0x02,0xdc,0x09,0xfb,0xdc,0x11,0x85,0x97,0x19,0x6a,0x0b,0x32};

  byte in[16],out[16];
  byte key[16];

  //printMes("in",inex);
  //printMes("key",keyex);

  for(i=0;i<16;i++) key[i]=keyex[i];
  for(i=0;i<16;i++) in[i]=inex[i];

  /*
  int k=2,ell=10;
  n=k*ell;
  byte x=10;
  byte a[n];
  int ind[k];

  encode_s(x,a,ind,k,ell);
  reshuffle_s(a,ind,k,ell);
  print_share_s(a,k,ell);
  for(int i=0;i<k;i++) printf("%d ",ind[i]);
  printf("\n");
  
  printf("%02x\n",decode_s(a,ind,k,ell));
  */

  int dt;
  float base;

  printf("Without countermeasure, plain: ");
  base=run_aes(&aes,in,out,key,outex,nt*100,0);

  printf("Without countermeasure, RP: ");
  run_aes(&aes_rp,in,out,key,outex,nt*100,base);

  base=base/100;

  int k=668;
  for(int ell=4;ell<=60;ell+=4)
  {
    int t=k*ell/4;
    n=t+1;
    printf("n=%d t=%d\n",n,n-1);
    printf("  With RP countermeasure:        ");
    run_aes_share(in,out,key,outex,n,nt,base);
  
    printf("k=%d ell=%d t=%d\n",k,ell,k*ell/4);
    printf("  With shuffling countermeasure: ");
    run_aes_share_s(in,out,key,outex,k,ell,nt,base);
    printf("\n");
  }
}  
