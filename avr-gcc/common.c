#include "common.h"

#include <stdlib.h>
//#include <time.h>
#include <string.h>
#include <avr/pgmspace.h>

void report_time(double dt,uint8_t nt,double base)
{
  //printf("%f %f\n",((double) dt)/CLOCKS_PER_SEC/nt*1000,dt/base);
  printf("Checkpoint - ReportTime\n");
}

void check_ciphertext(byte *out,const byte *outex,uint8_t nbyte)
{
  if(memcmp_PF(out,(uint_farptr_t)outex,nbyte)!=0)
  {
    fprintf(stderr,"Error: incorrect ciphertext\n");
    //exit(EXIT_FAILURE);
  }
}

double runalgo(void (*algo)(byte *,byte *,byte *),byte *in,byte *out,byte *key,const byte *outex,uint8_t nbyte,uint8_t nt,double base)
{
  uint8_t i;
  //clock_t start,end;
  double dt;
  //start=clock();
  for(i=0;i<nt;i++)
    algo(in,out,key);
  //end=clock();
  //dt=(double) (end-start);
  if (base==0.0f) base=dt;
  report_time(dt,nt,base);
  check_ciphertext(out,outex,nbyte);
  return dt;
}


void printMes(char *s,const byte *m)
{
  printf("%s=",s);
  uint8_t i;
  for(i=0;i<16;i++) printf("%02x",pgm_read_byte(&(m[i])));
  printf("\n");
}

void printMesSRAM(char *s,byte *m)
{
  printf("%s=",s);
  uint8_t i;
  for(i=0;i<16;i++) printf("%02x",m[i]);
  printf("\n");
}

void printStateShare(char *s,byte *m, uint8_t shares)
{
  printf("%s=",s);
  uint8_t i;
  for(i=0;i<shares;i++) printf("%02x",m[i]);
  printf("\n");
}

void printWShare(char *s,byte *m[176], uint8_t shares)
{
  printf("%s=",s);
  uint8_t i,j;
  for(i=0;i<176;i++) {
    printf("\n");
    for(j=0;j<shares;j++)
	printf("%02x",m[i][j]);
    
  }
  printf("\n");
}