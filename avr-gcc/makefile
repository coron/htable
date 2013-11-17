#microcontroller type for example atmega8
mcu = atmega8

AESOBJ = aes.o aes_rp.o share.o aes_share.o aes_htable.o runaes.o common.o

all : runaes.out

runaes.out : $(AESOBJ)
	avr-gcc -g -mmcu=$(mcu) -Os -o $@ $(AESOBJ)

%.o: %.c 
	avr-gcc -g -mmcu=$(mcu) -Os -o $@ -c $<

clean:
	rm -f *.o *.out
