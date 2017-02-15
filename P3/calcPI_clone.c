#define _GNU_SOURCE
#include <sched.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sched.h>
#include <stdio.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sched.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// 64kB stack
#define FIBER_STACK 1024 * 64
#define NITERATIONS 2000000000

#define CORES 4
long double n = 0;

// The child thread will execute this function
int threadFunction(void *argument)
{
    int arg = *(int *)argument;
    int iterations = (NITERATIONS / CORES) * arg;
    int step = NITERATIONS / CORES;
        for (int i = iterations; i < (step * (arg + 1)); ++i)
    {
        n+= (long double)pow(-1, i) / (2 * i + 1);
    }
}

int main()
{
    int i;
    void *stack[CORES];
    pid_t pid[CORES];
    int status;

    clock_t begin = clock();
    // Allocate the stack
    for (i = 0; i < CORES; i++)
    {
            stack[i] = malloc(FIBER_STACK);
        if (stack[i] == 0)
        {
            perror("malloc: could not allocate stack");
            exit(1);
        }
    }
    printf("Creating child thread\n");
    

    // Call the clone system call to create the child thread
    int iterations;
    for (i = 0; i < CORES; i++)
    {
        // change threadFunction for new function *************************************************
        pid[i] = clone(&threadFunction, (char *)stack[i] + FIBER_STACK, SIGCHLD | CLONE_SIGHAND | CLONE_FS | CLONE_VM, (void *)&i);
        if (pid[i] == -1)
        {
            perror("clone");
            exit(2);
        }
    }
    // Wait for the child thread to exit
    // pid = waitpid( pid, 0, 0 );
    for (i = 0; i < CORES; i++)
    {
        waitpid(pid[i], 0, 0);
        if (pid[i] == -1)
        {
            perror("waitpid");
            exit(3);
        }
    }

    for (i = 0; i < CORES; i++)
    {
        // Free the stack
        free(stack[i]);
    }
    
    printf("Child thread returned and stack freed. PI = %Lf\n", 4*n);
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("%f\n", time_spent);

    return 0;
}