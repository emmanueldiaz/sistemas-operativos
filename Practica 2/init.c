#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#define CHILDS 6

void createChild();
void childProcess(int processNum);

int main()
{
  pid_t pids[CHILDS], pid;
  char file[] = "exitcode";
  char completeFile[30];
  char num[5];
  int status;
  int finishedChilds = 0;
  int i;

  for (i = 0; i < CHILDS; i++)
  {
    if ((pids[i] = fork()) < 0)
    {
      printf("Could not fork process %d\n", i + 1);
    }
    else if (pids[i] == 0)
    {
      childProcess(i);
    }
  }

  while ((pid = wait(&status)) > 0)
  {
    for (i = 0; i < CHILDS; i++)
    {
      if (pid == pids[i])
      {
        FILE *fp;
        char code[10];
        sprintf(num, "%d", i);
        strcpy(completeFile, file);
        strcat(completeFile, num);
        fp = fopen(completeFile, "r");
        fscanf(fp, "%s", code);

        fclose(fp);
        remove(completeFile);

        char exitCode[] = "15";
        char shutdownCode[] = "50";

        if (strcmp(exitCode, code) == 0)
        {
          printf("Process Finished: %d\n", i + 1);
        }
        else if (strcmp(shutdownCode, code) == 0)
        {
          printf("All terminated: %d\n", i + 1);
          for (i = 0; i < CHILDS; i++)
          {
            kill(pids[i], SIGKILL);
          }
          exit(0);
        }
        else
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
  char fullCommand[50];
  char command[] = "./getty; echo $? >./exitcode";
  char num[10];
  sprintf(num, "%d", processNum);
  strcpy(fullCommand, command);
  strcat(fullCommand, num);
  execlp("xterm", "xterm", "-e", fullCommand, NULL);
}
