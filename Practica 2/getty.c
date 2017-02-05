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
      printf("Failed to fork process %d\n", i + 1);
    }
    else if (pids[i] == 0)
    {
      childProcess(i);
    }
  }
  printf("here end\n");
  while (finishedChilds < 5)
  {
    pid = wait(&status);
    printf("proceso %d termino con status %d\n", pid, status);
    finishedChilds++;
  }
}

void childProcess(int processNum)
{
  printf("proceso %d con pid %d\n", processNum, getpid());
  sleep(rand() % 10);
  exit(0);
}