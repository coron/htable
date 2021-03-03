// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License version 2 as published
// by the Free Software Foundation.

#include "aes_share.h"
#include "share.h"
#include "aes.h"
#include "aes_rp.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#define NOLOC 0
#define LOC 1

void shiftrows_share(byte *stateshare[16],int n)
{
  byte m;
  int i;
  for(i=0;i<n;i++)
  {
    m=stateshare[1][i];
    stateshare[1][i]=stateshare[5][i];
    stateshare[5][i]=stateshare[9][i];
    stateshare[9][i]=stateshare[13][i];
    stateshare[13][i]=m;

    m=stateshare[2][i];
    stateshare[2][i]=stateshare[10][i];
    stateshare[10][i]=m;
    m=stateshare[6][i];
    stateshare[6][i]=stateshare[14][i];
    stateshare[14][i]=m;

    m=stateshare[3][i];
    stateshare[3][i]=stateshare[15][i];
    stateshare[15][i]=stateshare[11][i];
    stateshare[11][i]=stateshare[7][i];
    stateshare[7][i]=m;
  }
}

void shiftrows_share_s(byte *stateshare[16],int *ind[16],int k,int ell)
{
  shiftrows_share(stateshare,k*ell);
  int m;
  for(int i=0;i<k;i++)
  {
    m=ind[1][i];
    ind[1][i]=ind[5][i];
    ind[5][i]=ind[9][i];
    ind[9][i]=ind[13][i];
    ind[13][i]=m;

    m=ind[2][i];
    ind[2][i]=ind[10][i];
    ind[10][i]=m;
    m=ind[6][i];
    ind[6][i]=ind[14][i];
    ind[14][i]=m;

    m=ind[3][i];
    ind[3][i]=ind[15][i];
    ind[15][i]=ind[11][i];
    ind[11][i]=ind[7][i];
    ind[7][i]=m;
  }
}

void mixcolumns_share(byte *stateshare[16],int n)
{
  byte ns[16];
  int i,j;
  for(i=0;i<n;i++)
  {
    for(j=0;j<4;j++)
    {
      ns[j*4]=multx(stateshare[j*4][i]) ^ multx(stateshare[j*4+1][i]) ^ stateshare[j*4+1][i] ^ stateshare[j*4+2][i] ^ stateshare[j*4+3][i];
      ns[j*4+1]=stateshare[j*4][i] ^ multx(stateshare[j*4+1][i]) ^ multx(stateshare[j*4+2][i]) ^ stateshare[j*4+2][i] ^ stateshare[j*4+3][i];
      ns[j*4+2]=stateshare[j*4][i] ^ stateshare[j*4+1][i] ^ multx(stateshare[j*4+2][i]) ^ multx(stateshare[j*4+3][i]) ^ stateshare[j*4+3][i];
      ns[j*4+3]=multx(stateshare[j*4][i]) ^ stateshare[j*4][i] ^ stateshare[j*4+1][i] ^ stateshare[j*4+2][i] ^ multx(stateshare[j*4+3][i]) ;
    }
    for(j=0;j<16;j++)
      stateshare[j][i]=ns[j];
  }
}

void mixcolumns_share_s(byte *s[16],int *ind[16],int k,int ell)
{
  for(int j=0;j<4;j++)
  {
    byte ns[16][k*ell];
    int nsi[16][k];

    byte temp[k*ell],temp2[k*ell];
    int tempi[k],tempi2[k];

    multx_s(s[j*4],ind[j*4],temp,tempi,k,ell);

    mult3_s(s[j*4+1],ind[j*4+1],temp2,tempi2,k,ell);

    xor_gate_s(temp,tempi,temp2,tempi2,ns[j*4],nsi[j*4],k,ell);
    xor_gate_s(ns[j*4],nsi[j*4],s[j*4+2],ind[j*4+2],ns[j*4],nsi[j*4],k,ell);
    xor_gate_s(ns[j*4],nsi[j*4],s[j*4+3],ind[j*4+3],ns[j*4],nsi[j*4],k,ell);

    multx_s(s[j*4+1],ind[j*4+1],temp,tempi,k,ell);
    mult3_s(s[j*4+2],ind[j*4+2],temp2,tempi2,k,ell);
    xor_gate_s(temp,tempi,temp2,tempi2,ns[j*4+1],nsi[j*4+1],k,ell);
    xor_gate_s(ns[j*4+1],nsi[j*4+1],s[j*4],ind[j*4],ns[j*4+1],nsi[j*4+1],k,ell);
    xor_gate_s(ns[j*4+1],nsi[j*4+1],s[j*4+3],ind[j*4+3],ns[j*4+1],nsi[j*4+1],k,ell);

    multx_s(s[j*4+2],ind[j*4+2],temp,tempi,k,ell);
    mult3_s(s[j*4+3],ind[j*4+3],temp2,tempi2,k,ell);
    xor_gate_s(temp,tempi,temp2,tempi2,ns[j*4+2],nsi[j*4+2],k,ell);
    xor_gate_s(ns[j*4+2],nsi[j*4+2],s[j*4],ind[j*4],ns[j*4+2],nsi[j*4+2],k,ell);
    xor_gate_s(ns[j*4+2],nsi[j*4+2],s[j*4+1],ind[j*4+1],ns[j*4+2],nsi[j*4+2],k,ell);

    multx_s(s[j*4+3],ind[j*4+3],temp,tempi,k,ell);
    mult3_s(s[j*4],ind[j*4],temp2,tempi2,k,ell);
    xor_gate_s(temp,tempi,temp2,tempi2,ns[j*4+3],nsi[j*4+3],k,ell);
    xor_gate_s(ns[j*4+3],nsi[j*4+3],s[j*4+1],ind[j*4+1],ns[j*4+3],nsi[j*4+3],k,ell);
    xor_gate_s(ns[j*4+3],nsi[j*4+3],s[j*4+2],ind[j*4+2],ns[j*4+3],nsi[j*4+3],k,ell);

    for(int i=0;i<4;i++)
    {
      memcpy(s[j*4+i],ns[j*4+i],k*ell);
      memcpy(ind[j*4+i],nsi[j*4+i],k*sizeof(int));
    }
  }
}

void addroundkey_share(byte *stateshare[16],byte *wshare[176],int round,int n)
{
  int i,j;
  for(i=0;i<16;i++)
    for(j=0;j<n;j++)
      stateshare[i][j]^=wshare[16*round+i][j];
}

void addroundkey_share_s(byte *stateshare[16],int *stateind[16],byte *wshare[176],int *wind[176],int round,int k,int ell)
{
  for(int i=0;i<16;i++)
    xor_gate_s(stateshare[i],stateind[i],wshare[16*round+i],wind[16*round+i],stateshare[i],stateind[i],k,ell);
}
  
void subbytestate_share(byte *stateshare[16],int n,void (*subbyte_share_call)(byte *,int))
{
  int i;
  for(i=0;i<16;i++)
    subbyte_share_call(stateshare[i],n);
} 

void subbytestate_share_s(byte *state[16],int *ind[16],int k,int ell)
{
  for(int i=0;i<16;i++)
    subbyte_rp_share_s(state[i],ind[i],k,ell);
}

void aes_share_subkeys(byte in[16],byte out[16],byte *wshare[176],int n)
{
  int i,j;
  int round=0;

  byte *stateshare[16];

  for(i=0;i<16;i++)
  {
    stateshare[i]=(byte*) malloc(n*sizeof(byte));
    share(in[i],stateshare[i],n);
  }  

  addroundkey_share(stateshare,wshare,0,n);

  for(round=1;round<10;round++)
  { 
    subbytestate_share(stateshare,n,subbyte_rp_share);
    shiftrows_share(stateshare,n);
    mixcolumns_share(stateshare,n);
    addroundkey_share(stateshare,wshare,round,n);
  }
 
  subbytestate_share(stateshare,n,subbyte_rp_share);
  shiftrows_share(stateshare,n);
  addroundkey_share(stateshare,wshare,10,n);

  for(i=0;i<16;i++)
  {
    out[i]=decode(stateshare[i],n);
    free(stateshare[i]);
  }
}

void aes_share_subkeys_s(byte in[16],byte out[16],byte *wshare[176],int *ind[176],int k,int ell)
{
  int i,round=0;

  byte *stateshare[16];
  int *stateind[16];

  for(i=0;i<16;i++)
  {
    stateshare[i]=(byte*) malloc(k*ell*sizeof(byte));
    stateind[i]=(int *) malloc(k*sizeof(int));
    encode_s(in[i],stateshare[i],stateind[i],k,ell);
  }  

  addroundkey_share_s(stateshare,stateind,wshare,ind,0,k,ell);
 
  for(round=1;round<10;round++)
  { 
    subbytestate_share_s(stateshare,stateind,k,ell);
    shiftrows_share_s(stateshare,stateind,k,ell);
     mixcolumns_share_s(stateshare,stateind,k,ell);
    addroundkey_share_s(stateshare,stateind,wshare,ind,round,k,ell);
  }
 
  subbytestate_share_s(stateshare,stateind,k,ell);
  shiftrows_share_s(stateshare,stateind,k,ell);
  addroundkey_share_s(stateshare,stateind,wshare,ind,round,k,ell);

  for(i=0;i<16;i++)
  {
    out[i]=decode_s(stateshare[i],stateind[i],k,ell);
    free(stateshare[i]);
    free(stateind[i]);
  }
}

void keyexpansion_share(byte key[16],byte *wshare[176],int n)
{
  byte w[176];
  keyexpansion(key,w);

  int randc=get_randcount();
  for(int i=0;i<176;i++)
  {
    wshare[i]=(byte *) malloc(n*sizeof(byte));
    share(w[i],wshare[i],n);
    refresh(wshare[i],n);
  }
  set_randcount(randc);
}


void keyexpansion_share_s(byte key[16],byte *wshare[176],int *wind[176],int k,int ell)
{
  byte w[176];
  keyexpansion(key,w);

  for(int i=0;i<176;i++)
  {
    wshare[i]=(byte *) malloc(k*ell*sizeof(byte));
    wind[i]=(int *) malloc(k*sizeof(int));
    encode_s(w[i],wshare[i],wind[i],k,ell);
  }
}
    
int run_aes_share(byte in[16],byte out[16],byte key[16],byte outex[16],int n,int nt,float base)
{
  byte *wshare[176];

  keyexpansion_share(key,wshare,n);

  init_randcount();
  clock_t start=clock();

  for(int i=0;i<nt;i++)
    aes_share_subkeys(in,out,wshare,n);
  clock_t end=clock();

  for(int i=0;i<176;i++)
    free(wshare[i]);

  int dt= (int) (end-start);
  report_time(dt,nt,base,get_randcount());
  check_ciphertext(out,outex,16);
  return dt;
}

int run_aes_share_s(byte in[16],byte out[16],byte key[16],byte outex[16],int k,int ell,int nt,float base)
{
  byte *wshare[176];
  int *wind[176];

  keyexpansion_share_s(key,wshare,wind,k,ell);
 
  clock_t start=clock();
  for(int i=0;i<nt;i++)
    aes_share_subkeys_s(in,out,wshare,wind,k,ell);
  clock_t end=clock();

  for(int i=0;i<176;i++)
  {
    free(wshare[i]);
    free(wind[i]);
  }
  
  int dt= (int) (end-start);
  report_time(dt,nt,base,get_randcount());
  check_ciphertext(out,outex,16);

  return 0;
}
