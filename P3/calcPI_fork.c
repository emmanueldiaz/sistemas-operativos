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
#include <math.h>
#include <time.h>

// 64kB stack
#define FIBER_STACK 1024 * 64
#define NITERATIONS 2000000000
#define CORES 4

int n = 0;

FILE *fp = NULL; 


// The child thread will execute this function
int threadFunction(int arg)
{
    char towrite[10];
    char completeFile[30];
    char file[] = "result";
    char num[5];
    sprintf(num, "%d", arg);
    strcpy(completeFile, file);
    strcat(completeFile, num);
    int iterations = (NITERATIONS / CORES) * arg;
    int step = NITERATIONS / CORES;
    double fourthPI = 0;
    for (int i = iterations; i < (step * (arg + 1)); ++i)
    {
        fourthPI += (double)pow(-1, i) / (2 * i + 1);
    }
    sprintf(towrite, "%1.13f", fourthPI*4);
    //CREATE FILE HERE WITH RESULT
    fp = fopen(completeFile, "w");
    strcat(towrite, "\n");
    fputs(towrite, fp);
    fclose(fp);
    exit(0);
}

int main()
{
    pid_t pids[CORES], pid;
    char completeFile[30];
    char num[5];
    char file[] = "result";
    int status;
    int finishedChilds = 0;
    int i;
    double result;
    double total = 0;

    clock_t begin = clock();

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
        char *ptr;
        sprintf(num, "%d", i);
        strcpy(completeFile, file);
        strcat(completeFile, num);
        fp = fopen(completeFile, "r");
        fscanf(fp, "%s", code);

        //make code an int
        result = strtod(code, &ptr);//conversion of code;
        total += result;
        fclose(fp);
        remove(completeFile);
    }
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("%f\n", time_spent);
    printf("%f\n", total);
}