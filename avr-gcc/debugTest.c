

#include<stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>

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

void outs(const char *str) {
  const char *c;

  for(c = str; *c; c++)
    special_output_port = *c;
}

static FILE my_stdout = FDEV_SETUP_STREAM(output_char, NULL, _FDEV_SETUP_WRITE);


/* Main for test program.  Enter a string and echo it. */
int main() {
    char buf[16];
    
sei();
  stdout = &my_stdout;
          sprintf(buf, "salam\n");
outs(buf);
  /* Output the prompt string */
  printf("Press any k> ");

  return 0;
}
