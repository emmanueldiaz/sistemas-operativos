#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
int main(int argc, char *argv[])
{
	FILE *fp;
	char coladest[10];
	// Valida que reciba el argumento
	if(argc!=2)
	{
		perror("Error en los argumentos\n");
		exit(EXIT_FAILURE);
	}
	sprintf(coladest,"fifo_%d",atoi(argv[1]));
	printf("Trabajo a enviarse en %s\n",coladest);
	// Abre el pipe del servidor 1 en modo de escritura
	fp=fopen(coladest,"w+");
	// Escribe en el pipe, en este caso el pid del proceso
	fprintf(fp,"%d",getpid());
	fclose(fp);
}
