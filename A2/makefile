CC= gcc
CFLAGS= -ggdb -Wall -pedantic -std=c11

source : source.o printStats.o

source.o : file_util.h

printStats.o : a2.h

clean:
	rm -f *.o core source

run :
	./source ./a2in.txt
