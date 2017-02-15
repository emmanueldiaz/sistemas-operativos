#define _GNU_SOURCE
#include <sched.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sched.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>

// 64kB stack
#define FIBER_STACK 1024 * 64
#define NITERATIONS 2000000000
#define CORES 4

int n = 0;

// The child thread will execute this function
int threadFunction(int arg)
{
    int iterations = (NITERATIONS / CORES) * arg;
    int step = NITERATIONS / CORES;
    long double fourthPI = 0;
    clock_t begin = clock();
    for (int iterations = iterations; i < (step * (arg + 1)); ++i)
    {
        fourthPI += (long double)pow(-1, i) / (2 * i + 1);
    }
    //CREATE FILE HERE WITH RESULT
    clock_t end = clock();
    printf("%Lf\n", 4 * fourthPI);
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("%f\n", time_spent);
    exit(0);
}

int main()
{
    pid_t pids[CORES], pid;
    char completeFile[30];
    char file[] = "result";
    char num[5];
    int status;
    int finishedChilds = 0;
    int i;
    int total = 0;

    for (i = 0; i < CORES; i++)
    {
        if ((pids[i] = fork()) < 0)
        {
            printf("Could not fork process %d\n", i + 1);
        }
        else if (pids[i] == 0)
        {
            threadFunction(i);
        }
    }

    int completeProces = 0;
    while ((pid = wait(&status)) > 0)
    {

        FILE *fp;
        char code[10];
        sprintf(num, "%d", i);
        strcpy(completeFile, file);
        strcat(completeFile, num);
        fp = fopen(completeFile, "r");
        fscanf(fp, "%s", code);

        //make code an int
        result = //conversion of code;
        total + = result
        fclose(fp);
        remove(completeFile);
    }
}