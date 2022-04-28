# file:		Makefile
# author:	garnt
# date:		04/24/2022
# desc:		Makefile to build house_of_delegates challenges.

C=gcc

all: clean libgarntalloc.so house_of_delegates elected_felon

clean:
	rm -f libgarntalloc.so test_garntalloc house_of_delegates elected_felon

libgarntalloc.so:
	cp ../target/debug/libgarntalloc.so libgarntalloc.so

test_garntalloc: libgarntalloc.so garntalloc.h test_garntalloc.c
	$(C) test_garntalloc.c -o test_garntalloc -Wl,-rpath='$$ORIGIN' -L . -lgarntalloc

house_of_delegates: libgarntalloc.so garntalloc.h house_of_delegates.c
	$(C) house_of_delegates.c -o house_of_delegates -Wl,-rpath='$$ORIGIN' -L . -lgarntalloc -ldl

elected_felon: libgarntalloc.so garntalloc.h elected_felon.c
	$(C) elected_felon.c -o elected_felon -Wl,-rpath='$$ORIGIN' -L . -lgarntalloc
