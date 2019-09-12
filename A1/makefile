CC= gcc
CFLAGS= -ggdb -Wall -pedantic -std=c11

source : source.o readfile.o interactive.o

source.o : readfile.h interactive.h structs.h

readfile.o : readfile.h structs.h

interactive.o : interactive.h structs.h

clean:
	rm -f *.o core source

run:
	./source