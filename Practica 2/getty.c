#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc,char *argv[])
{
	int i;
	printf("Hola mundo, mi pid es %d\n",getpid());
	for(i=0;i<argc;i++)
		printf("%s\n",argv[i]);
}