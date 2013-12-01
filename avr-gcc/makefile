#microcontroller type for example atmega8
mcu = atmega128

AESOBJ = aes.o aes_rp.o share.o aes_share.o aes_htable.o runaes.o common.o
#AESOBJ_norm = aes.o aes_rp.o share.o aes_share.o aes_htable.o runaes_normal.o common.o


all : runaes.elf

runaes.elf : $(AESOBJ)
	avr-gcc -g -mmcu=$(mcu) -O3 -o $@ $(AESOBJ)


#runaes_normal.out : $(AESOBJ_norm)
#        avr-gcc -g -mmcu=$(mcu) -Os -o runaes_normal.out $(AESOBJ_norm)

%.o: %.c 
	avr-gcc -g -mmcu=$(mcu) -Os -o $@ -c $<

clean:
	rm -f *.o *.elf
