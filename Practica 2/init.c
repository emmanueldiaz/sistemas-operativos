#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define CHILDS 6

void createChild();
void childProcess(int processNum);

int main()
{
  pid_t pids[CHILDS], pid;

  int status;
  int finishedChilds = 0;
  int i;

  for (i = 0; i < CHILDS; i++)
  {
    if ((pids[i] = fork()) < 0)
    {
      printf("No de pudo hacer fork del proceso %d\n", i + 1);
    }
    else if (pids[i] == 0)
    {
      childProcess(i);
    }
  }
  while (finishedChilds < CHILDS)
  {
    pid = wait(&status);
    printf("proceso %d termino con status %d\n", pid, status);
    finishedChilds++;
  }
}

void childProcess(int processNum)
{
  printf("%s\n", "Opening thing" );
  execlp("xterm", "-e", "./getty", NULL);
  exit(0);
}