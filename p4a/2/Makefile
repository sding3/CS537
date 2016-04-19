.PHONY: all
all : libcrawler.so file_tester web_tester busywait

busywait : busywait.c libcrawler.so 
	gcc -g -L. -lcrawler -lpthread busywait.c -Wall -Werror -o busywait

file_tester : file_tester.c libcrawler.so 
	gcc -g -L. -lcrawler -lpthread file_tester.c -Wall -Werror -o file_tester

web_tester : web_tester.c cs537.c libcrawler.so
	gcc -g -L. -lcrawler -lpthread web_tester.c cs537.c -Wall -Werror -o web_tester

libcrawler.so : crawler.c libha.so
	gcc -g -L. -lha -fpic -c crawler.c -Wall -Werror -o crawler.o
	gcc -g -shared -o libcrawler.so libha.so crawler.o

libha.so : ha.c
	gcc -g -fpic -c ha.c -Wall -Werror -o ha.o
	gcc -g -shared -o libha.so ha.o

.PHONY: clean
clean :
	rm -f file_tester web_tester busywait libcrawler.so libha.so *.o *~
