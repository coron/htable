#ifndef __common_h__
#define __common_h__

#include <stdio.h>


typedef uint8_t byte;

////////////////////////////////
/* This port correponds to the "-W 0x20,-" command line option. */
#define special_output_port (*((volatile char *)0x20))
/* This port correponds to the "-R 0x22,-" command line option. */
#define special_input_port  (*((volatile char *)0x22))

/* Poll the specified string out the debug port. */
static int output_char(char c, FILE *stream)
{
    special_output_port = c;
    return 0;
}

static FILE my_stdout = FDEV_SETUP_STREAM(output_char, NULL, _FDEV_SETUP_WRITE);
////////////////////////////////

void report_time(double dt,int nt,double base);
void check_ciphertext(byte *out,byte *outex,int nbyte);
double runalgo(void (*algo)(byte *,byte *,byte *),byte *in,byte *out,byte *key,byte *outex,int nbyte,int nt,double base);

#endif
