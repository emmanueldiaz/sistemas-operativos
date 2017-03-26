#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sem.h>


#define MAXSERVERS 3

int semaforo[MAXSERVERS];

int main()
{
	int i;
	int pid;
	int status;
	char numserver[10];
	
	for(i=0;i<MAXSERVERS;i++)
	{
		semaforo[i]=semget((key_t)0x1234,1,0666|IPC_CREAT); // Pedir al SO que me de un semáforo
		semctl(semaforo[i],0,SETVAL,1);	// Inicializa el elemento 0 del arreglo de semáforos con 1

		pid=fork();
		if(pid==0)
		{
			sprintf(numserver,"%d",i);
			execlp("xterm", "xterm", "-e", "./servermsg",
			numserver,NULL);
		}
	}
	for(i=0;i<MAXSERVERS;i++)
		wait(&status);
}
