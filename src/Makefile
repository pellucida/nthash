


OBJ	= md4.o nthash.o
CFLAGS	= -I.

nthash:	$(OBJ) 

test:
	@echo "81D0C53127E605B0359C864767E98153" expected
	@./nthash "The Jaberwocky9999"

clean:
	rm -f a.out *.o
##
md4.o: md4.c md4.h
nthash.o: nthash.c md4.h
