CC= gcc
CFLAGS= -pthread -ggdb -Wall -pedantic 

all: source_multiprocessed source_threaded

clean:
	rm -f *.o core source_multiprocessed source_threaded

run :
	./source_multiprocessed
	./source_threaded
