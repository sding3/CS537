.PHONY: all
all : tester libmem.so

tester : tester.c
	gcc -L. -lmem tester.c -Wall -Werror -o tester

libmem.so : mem.c
	gcc -fpic -c mem.c -Wall -Werror -o mem.o
	gcc -shared -o libmem.so mem.o
