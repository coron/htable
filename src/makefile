AESOBJ = aes.o aes_rp.o share.o aes_share.o aes_htable.o runaes.o common.o

DESOBJ = share.o des_carlet.o des_crv.o des.o rundes.o des_htable.o des_share.o common.o

all : rundes.out runaes.out

rundes.out : $(DESOBJ)
	gcc -O3 -o $@ $(DESOBJ)

runaes.out : $(AESOBJ)
	gcc -O3 -o $@ $(AESOBJ)

%.o: %.c 
	gcc -O3 -o $@ -c $<

clean:
	rm -f *.o *.out
