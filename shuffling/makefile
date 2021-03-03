AESOBJ = aes.o aes_rp.o share.o aes_share.o runaes.o common.o 

runaes.out : $(AESOBJ)
	gcc -O3 -o $@ $(AESOBJ)

shuffling.tar.gz :
	tar -zcvf shuffling.tar.gz *.c *.h *.sage *.dat makefile 

%.o: %.c 
	gcc -O3 -o $@ -c $<

clean:
	rm -f *.o *.out
