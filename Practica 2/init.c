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
  while ((pid = wait(&status)) > 0)
  {
    printf("Exit status of %d was %d\n", (int)pid, status);
    if (status == 0)
    {
      for (i = 0; i < CHILDS; i++)
      {
        if (pid == pids[i])
        {
          printf("Restarting process: %d\n", i + 1);
          pids[i] = fork();
          if (pids[i] == 0)
          {
            childProcess(i);
          }
        }
      }
    }
  }
}

void childProcess(int processNum)
{
  execlp("xterm", "-e", "./getty", NULL);
  printf("%s\n", "closing thing");

  exit(0);
}