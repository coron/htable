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
#include "aes_rp_prg.h"
#include "share.h"
#include "aes_share.h"
#include "aes_htable.h"
#include "common.h"
#include "prg.h"


void printMes(char *s,byte *m)
{
  printf("%s=",s);
  int i;
  for(i=0;i<16;i++) printf("%02x",m[i]);
  printf("\n");
}

int main()
{
  int n;
  int nt=10;
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

  int dt,base;

  printf("Without countermeasure, plain: ");
  base=run_aes(&aes,in,out,key,outex,nt,0);

  printf("Without countermeasure, RP: ");
  run_aes(&aes_rp,in,out,key,outex,nt,base);

  for(n=3;n<=6;n+=1)
  {
    printf("n=%d\n",n);
    printf("  With RP countermeasure: ");
    run_aes_share(in,out,key,outex,n,&subbyte_rp_share,nt,base);
    
    printf("  With RP countermeasure, with flr: ");
    int rprg=rprg_flr(n);
    run_aes_share_prg(in,out,key,outex,n,&subbyte_rp_share_flr,base,nt,rprg);
    printf(" trand=%d tprgcount=%d\n",rprg*2*n,(480*n+1120)*(n-1));

    printf("  With RP countermeasure, with ilr: ");
    rprg=rprg_ilr(n);
    run_aes_share_prg(in,out,key,outex,n,&subbyte_rp_share_ilr,base,nt,rprg);
    printf(" trand=%d tprgcount=%d\n",8*n*(n-1)*(n-1),(960*n+160)*(n-1));

    printf("  With RP countermeasure, with ilr2: ");
    rprg=rprg_ilr(n);
    run_aes_share_prg(in,out,key,outex,n,&subbyte_rp_share_ilr2,base,nt,rprg);
    printf(" trand=%d tprgcount=%d\n",8*n*(n-1)*(n-1),(480*n+1120)*(n-1));

    printf("  With RP countermeasure, with flr, multiple prg: ");
    run_aes_share_mprg(in,out,key,outex,n,&subbyte_rp_share_flr_mprg,TFLR,base,nt);
    printf(" trand=%d\n",(n*n+9*n-10)*(n-1));

    printf("  With RP countermeasure, with ilr, multiple prg: ");
    run_aes_share_mprg(in,out,key,outex,n,&subbyte_rp_share_ilr_mprg,TILR,base,nt);
    printf(" trand=%d\n",(12*n-12)*(n-1));

    if(n<=4)
    {
      printf(" With RP countermeasure, with flr, mprgmat: ");
      run_aes_share_mprgmat(in,out,key,outex,n,base,nt);
      printf(" predicted rand: %d\n",n*(n-1)/2*2*31+3*(n-1)*2*38);
    }
    
    printf("  With randomized table : ");
    run_aes_share(in,out,key,outex,n,&subbyte_htable,base,nt); 

    printf("  With randomized table inc: ");
    run_aes_share(in,out,key,outex,n,&subbyte_htable_inc,base,nt); 

    printf("  With randomized table word: ");
    run_aes_share(in,out,key,outex,n,&subbyte_htable_word,base,nt);

    printf("  With randomized table word inc: ");
    run_aes_share(in,out,key,outex,n,&subbyte_htable_word_inc,base,nt); 

    printf("  With randomized table common shares: ");
    run_aes_common_share(in,out,key,outex,n,&subbyte_cs_htable,base,nt); 
    
    printf("  With randomized table word common shares: ");
    run_aes_common_share(in,out,key,outex,n,&subbyte_cs_htable_word,base,nt); 

    printf("  With randomized table word inc common shares: ");
    run_aes_common_share(in,out,key,outex,n,&subbyte_cs_htable_word_inc,base,nt); 
  }
}


