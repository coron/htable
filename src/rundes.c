// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License version 2 as published
// by the Free Software Foundation.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "share.h"
#include "des.h"
#include "des_carlet.h"
#include "des_crv.h"
#include "des_htable.h"
#include "des_share.h"

void test_des_encrypt()
{
  int n;
  int nt=1000;
  int i;
  byte key[8]={0x5b,0x5a,0x57,0x67,0x6a,0x56,0x67,0x6e};
  byte inex[8]={0x67,0x5a,0x69,0x67,0x5e,0x5a,0x6b,0x5a};
  byte outex[8]={0x97,0x4a,0xff,0xbf,0x86,0x02,0x2d,0x1f}; 

  byte in[8];
  byte out[8];

  //  testlagrangeRoy();
    
  for(i=0;i<8;i++) in[i]=inex[i];

  int dt,base=0;

  printf("Without countermeasure, plain: ");
  dt=run_des(in,out,key,nt);
  base=dt;
  report_time(dt,nt,base,0);
  check_ciphertext(out,outex,8);

  printf("Without countermeasure, Carlet: ");
  runalgo(des_encrypt_carlet,in,out,key,outex,8,nt,base);

  for(n=3;n<=10;n++)
  {
    printf("n=%d\n",n);
    printf("  With Carlet (RV13) countermeasure: ");
    init_randcount();
    dt=run_des_share(in,out,key,n,&polyRoy_share,nt); // &polygen_share
    report_time(dt,nt,base,get_randcount());
    check_ciphertext(out,outex,8);

    printf("  With Carlet (CRV14) countermeasure: ");
    init_randcount();
    dt=run_des_share(in,out,key,n,&polyCRV_share,nt);
    report_time(dt,nt,base,get_randcount());
    check_ciphertext(out,outex,8);	
	
    printf("  With randomized table: ");
    init_randcount();
    dt=run_des_share(in,out,key,n,&sbox_htable_word,nt);
    report_time(dt,nt,base,get_randcount());
    check_ciphertext(out,outex,8);

    printf("  With randomized table inc: ");
    init_randcount();
    dt=run_des_share(in,out,key,n,&sbox_htable_word_inc,nt);
    report_time(dt,nt,base,get_randcount());
    check_ciphertext(out,outex,8);
  }
}

int main()
{
  test_des_encrypt();
}
