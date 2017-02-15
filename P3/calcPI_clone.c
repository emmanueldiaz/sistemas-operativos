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

// 64kB stack
#define FIBER_STACK 1024 * 64

int n = 0;

// The child thread will execute this function
int threadFunction(void *argument)
{
    int arg = *(int *)argument;
    n += 5;
    argument++;
    printf("Inicia hilo %d\n", arg);
    printf("child thread exiting\n");
    return 0;
}

int main()
{
    int i;
    void *stack[4];
    pid_t pid[4];
    int status;

    // Allocate the stack
    for (i = 0; i < 4; i++)
    {
        å
            stack[i] = malloc(FIBER_STACK);
        if (stack[i] == 0)
        {
            perror("malloc: could not allocate stack");
            exit(1);
        }
    }
    printf("Creating child thread\n");

    // Call the clone system call to create the child thread

    for (i = 0; i < 4; i++)
    {
        // change threadFunction for new function *************************************************
        // change i for the amount of cycles and range to iterate *********************************
        pid[i] = clone(&threadFunction, (char *)stack[i] + FIBER_STACK, SIGCHLD | CLONE_SIGHAND | CLONE_FS | CLONE_VM, (void *)&i);
        if (pid[i] == -1)
        {
            perror("clone");
            exit(2);
        }
    }
    // Wait for the child thread to exit
    // pid = waitpid( pid, 0, 0 );
    for (i = 0; i < 4; i++)
    {

        waitpid(pid[i], 0, 0);
        if (pid[i] == -1)
        {
            perror("waitpid");
            exit(3);
        }
    }

    for (i = 0; i < 4; i++)
    {
        // Free the stack
        free(stack[i]);
    }
    printf("Child thread returned and stack freed. Total n = %d\n", n);

    return 0;
}