#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>


void getTokens(char* p_string);
void removeQuote(char* p_string, char c);
void setCommand (char *p_string);
int setVariable ();
char* getVariable(char * arrPosition);
void execProgram(char * p_string);

char *p_tokens[15];
char *arrString;
int status = 0; 

int main(){
	char cline_buff[256];
	char *input = cline_buff;
	int j,i;
	while (1){
		for(j = 0; j<15;j++)
			p_tokens[j]=NULL;
		printf("sh > ");
		fgets(cline_buff, 256, stdin);
		getTokens(input);
		setCommand (p_tokens[0]);
	}
	return 0;

}

//Decodificar instruccion
void setCommand (char * p_string)
{
	if (strncmp(p_string,"exit",4)==0){
				status = 10;
				exit(0);
	}
	else if (strncmp (p_string,"shutdown",8)==0)
		status = 50;
	else if (strncmp (p_string,"set",3)==0)
		setVariable();
	else if (strncmp (p_string,"echo",4)==0)
		printf ("%s\n",p_tokens[1]);
	else
		execProgram(p_string);

}

void execProgram(char * p_string)
{
	if(p_tokens[1]==NULL && p_tokens[0])
		p_string[strlen(p_string)-1] ='\0';
	int pid;
	pid = fork ();
	if (pid==0){
		execlp("nice","Error","--adjustment=0",p_string,0,NULL);		

		return;
	}else{
		wait(NULL);	
	}
}



int setVariable ()
{
	p_tokens[1][strlen(p_tokens[1])-1] ='\0';
	char * var=(char*) malloc(sizeof(char)*strlen(p_tokens[1]));
	int i;
	int len = strlen (p_tokens[1]);
	for (i=0;i<len;i++)
		var[i]=p_tokens[1][i];
    	int r = putenv(var);
}

char* getVariable (char * arrPosition)
{
	char * envVar;
	memmove(arrPosition, arrPosition+1, strlen(arrPosition));
	arrPosition[strlen(arrPosition)-1] ='\0';
	envVar = getenv(arrPosition);
	char * var=(char*) malloc(1 + strlen(envVar));
	if(envVar!=NULL){
		envVar = envVar;
		strcpy(var,envVar);
		strcat(var, " ");
		return var;	
	}else
		envVar = "Undefined var\n";
	return envVar;
}


void getTokens(char* p_string){
	int i = 0;
	char *token;
	for (token = strtok(p_string, " "); token; token = strtok(NULL, " "))
	{
	    p_tokens[i] = token;
	    removeQuote (p_tokens[i]);
	    if (p_tokens[i][0]=='$')
		p_tokens[i]=getVariable(p_tokens[i]);
	    i++;
	}
	p_tokens[i] = NULL;
}

void removeQuote(char* p_string) {
    char *pr = p_string, *pw = p_string;
    char c = '\"'
    while (*pr) {
        *pw = *pr++;
        pw += (*pw != c);
    }
    *pw = '\0';
}