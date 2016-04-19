#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void strcatt(char *s, char *t){
	int len = strlen(s);
	s = s + len;
	int i;
	for(;*t!='\0';t++,s++){
		printf("%c\n",*t);
		*s = *t;
		printf("%c\n",*s);
	}
	*s = '\0';
}
void strcatg(char s[], char t[])
{
	int i, j;
	i = j = 0;
	while (s[i] != '\0') /* find end of s */
		i++;
	while ( (s[i++] = t[j++]) != '\0')
		i;
}	
int
main(int argc,char *argv[]){
	char *s = "hello ";
	char *t = "world";
	char *f = strcat(s,t);
	printf("%s\n",f);
	return 0;
}

