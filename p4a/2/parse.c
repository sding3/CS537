#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    char string[] = "sd linkfileZ asdsad\nasdw link:fileb\n asldkjijldkdj link:filed";
    char *token;
    char *save;
 
 	for(token = strtok_r(string, " \n", &save);
						token!=NULL;
						token=strtok_r(NULL, " \n", &save)){
		if(strncmp("link:",token ,5)==0)
			printf("%s\n",token+5);
		
	}

	return 0;
}	

//int strncmp(const char *s1, const char *s2, size_t n);
