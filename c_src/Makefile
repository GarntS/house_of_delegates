# file:		Makefile
# author:	garnt
# date:		04/24/2022
# desc:		Makefile to build house_of_delegates challenges.

LIB=-L../target/release/libgarntalloc.so

all: clean libgarntalloc.so test_garntalloc

clean:
	rm -f libgarntalloc.so test_garntalloc

libgarntalloc.so:
	cp ../target/debug/libgarntalloc.so libgarntalloc.so

test_garntalloc: libgarntalloc.so garntalloc.h test_garntalloc.c
	gcc test_garntalloc.c -o test_garntalloc -L . -lgarntalloc 