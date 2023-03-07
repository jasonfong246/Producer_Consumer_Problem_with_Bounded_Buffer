# target all means all targets currently defined in this file
all: shell.zip shell.c

WARNS=-Wall -pedantic -Wextra
CFLAGS=-g3 -std=gnu11 ${WARNS}
LIBS=-lpthread -lrt

# this target is the .zip file that must be submitted to Carmen
lab2.zip: Makefile main.c
	zip -r lab2.zip buffer.h main.c Makefile README.txt headers.sh headers.awk

main: main.o
	gcc ${CFLAGS} main.o -o main ${LIBS}

main.o: main.c buffer.h
	gcc -ansi -pedantic -g -c main.c

# this target deletes all files produced from the Makefile
# so that a completely new compile of all items is required
clean:
	rm -rf *.o main lab2.zip
