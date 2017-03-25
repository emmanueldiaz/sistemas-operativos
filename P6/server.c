#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
int numserver;
void procesa(char *buffer);
int main(int argc,char *argv[])
{
int fp;
int i;
char buffer[10];
char namefifo[10];
if(argc!=2)
{
perror("Indica el número de servidor\n");
exit(EXIT_FAILURE);
}
numserver=atoi(argv[1]);
printf("Iniciando server %d\n",numserver);
// Inicializa la secuencia de números random
srand((unsigned int)getpid());
// Nombra el fifo_n donde n es el número del servidor
sprintf(namefifo,"fifo_%d",numserver);
// Elimina y crea un nuevo pipe a través del cual recibe datos del cliente
unlink(namefifo);
mkfifo(namefifo,0644);
for(i=0;i<10;i++)
{
memset(buffer,0,10);
// Abre el pipe fifo_n
fp=open(namefifo,0);
// Lee info del pipe
read(fp,buffer,sizeof(buffer));
// Función que procesa al cliente
procesa(buffer);
// Cierra el pipe
close(fp);
}
exit(0);
}
void procesa(char *buffer)
{
printf("Inicia trabajo %s por servidor %d\n",buffer,numserver);
sleep(rand()%5);
printf("Termina trabajo %s por servidor %d\n",buffer,numserver);
}
