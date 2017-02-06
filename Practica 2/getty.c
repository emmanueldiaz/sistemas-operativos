#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc,char *argv[])
{
	int i;
	char user[50], pass[50];
	printf("Hola mundo, mi pid es %d\n",getpid());
	printf("%s\n","enter user id");
	scanf("%s", user);
}