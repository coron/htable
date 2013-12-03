#include <stdlib.h>
//#include <time.h>
#include <string.h>


#include "aes.h"
#include "aes_rp.h"
#include "share.h"
#include "aes_share.h"
//#include "aes_htable.h"
#include "common.h"



void printMes(char *s,byte *m)
{
  printf("%s=",s);
  uint8_t i;
  for(i=0;i<16;i++) printf("%02x",m[i]);
  printf("\n");
}

int main()
{
  //sets up stdout for debug
  stdout = &my_stdout;
  stderr = &my_stdout;
  
  uint8_t n;
  uint8_t nt=100;
  uint8_t i;
  byte keyex[16]={0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};

  byte inex[16]={0x32,0x43,0xf6,0xa8,0x88,0x5a,0x30,0x8d,0x31,0x31,0x98,0xa2,0xe0,0x37,0x07,0x34};

  byte outex[16]={0x39,0x25,0x84,0x1d,0x02,0xdc,0x09,0xfb,0xdc,0x11,0x85,0x97,0x19,0x6a,0x0b,0x32};

  byte in[16],out[16];
  byte key[16];

  printMes("in",inex);
  printMes("key",keyex);

  for(i=0;i<16;i++) key[i]=keyex[i];
  for(i=0;i<16;i++) in[i]=inex[i];

  double dt,base;

  printf("Without countermeasure, plain: ");
  dt=run_aes(in,out,key,nt);
  base=dt;
  check_ciphertext(out,outex,16);
  report_time(dt,nt,base);

  printf("Without countermeasure, RP: ");
  runalgo(&aes_rp,in,out,key,outex,16,nt,base);

  for(n=3;n<=9;n+=2)
  {
    printf("n=%d\n",n);
    
    printf("  With RP countermeasure: ");
    dt=run_aes_share(in,out,key,n,&subbyte_rp_share,nt);
    report_time(dt,nt,base);
    check_ciphertext(out,outex,16);
/*
    printf("  With randomized table : ");
    dt=run_aes_share(in,out,key,n,&subbyte_htable_word,nt);
    report_time(dt,nt,base);
    check_ciphertext(out,outex,16);
*/
  }

}


