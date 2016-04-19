.PHONY : all

all : tester libmem.so

tester : tester.c
	gcc -L. -Wall -Werror tester.c -o tester -lmem

libmem.so : mem.c
	gcc -c -Wall -Werror -fpic mem.c
	gcc -shared -o libmem.so mem.o
clean :
	rm *.o *.so tester
