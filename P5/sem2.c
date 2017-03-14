#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include  <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include "queue.h"


#define atomic_xchg(A, B) __asm__ __volatile__( \
    " lock xchg %1,%0 ;\n"                      \
    : "=ir"(A)                                  \
    : "m"(B), "ir"(A));

#define CICLOS 10
#define MAX 20

struct sem {
    int count;
    struct QUEUE *blocked;
};

char *pais[3] = {"Peru", "Bolivia", "Colombia"};
int *g, *h;
struct sem *sem;

//Tomado de https://www.tutorialspoint.com/data_structures_algorithms/queue_program_in_c.htm

int isEmpty(struct QUEUE q) {
    if (q.itemCount == 0)
    {
        return 1;
    }
   return 0;
}

int isFull(struct QUEUE q) {
    if (q.itemCount == MAX)
    {
        return 1;
    }
   return 0;
}

int size(struct QUEUE q) {
   return q.itemCount;
}  

void insert(struct QUEUE *q, int data) {

   if(!isFull(*q)) {
    
      if(q->rear == MAX-1) {
         q->rear = -1;            
      }       
      q->rear++;
      q->intArray[q->rear] = data;
      q->itemCount++;
   }
}

int removeData(struct QUEUE *q) {
    q->front++;
   int data = q->intArray[q->front];
   
   if(q->front == MAX) {
      q->front = 0;
   }
    
   q->itemCount--;
   return data;  
}

void initsem(struct sem *s, int value) {
    s->count = value;
}

void waitsem(struct sem *s) {
    int l = 1;
    do { atomic_xchg(l,*h); } while(l!=0);
    if(s->count <= 0){
          insert(sem->blocked, getpid());
          kill(getpid(), SIGSTOP);
    }
    s->count--;
    *h = 0;
}

void signalsem(struct sem *s) {
    int process;
    int l = 1;
    do { atomic_xchg(l,*g); } while(l!=0);
    s->count++;
    if(!isEmpty(*sem->blocked)){
        process = removeData(sem->blocked);
        *g = 0;
        kill(process, SIGCONT);
    }
    *g = 0;
}

void proceso(int i)
{
    int k;
    int l;
    for (k = 0; k < CICLOS; k++)
    {
        waitsem(sem);
        printf("Entra %s", pais[i]);
        fflush(stdout);
        sleep(rand() % 3);
        printf("- %s Sale\n", pais[i]);
       signalsem(sem);
        // Espera aleatoria fuera de la sección crítica
        sleep(rand() % 3);
    }
    exit(0); // Termina el proceso
}
int main()
{
    int pid;
    int status;
    int shmid;
    int args[3];
    int i;
    void *thread_result;

// Request semaphore shared mem for the semaphore and the queue
    shmid = shmget(0x1235, sizeof(struct sem), IPC_CREAT | 0666);
    sem = shmat(shmid, NULL, 0);
    shmid = shmget(0x1236, sizeof(struct QUEUE), IPC_CREAT | 0666);
    sem->blocked = shmat(shmid, NULL, 0);
//init queue values
    sem->blocked->front = 0;
    sem->blocked->rear = -1;
    sem->blocked->itemCount = 0;
//init semaphore count value
    initsem(sem, 1);

    // Solicitar memoria compartida
    shmid = shmget(0x1234, sizeof(g), 0666 | IPC_CREAT);
    if (shmid == -1)
    {
        perror("Error en la memoria compartida\n");
        exit(1);
    }
    // Conectar la variable a la memoria compartida
    g = shmat(shmid, NULL, 0);
    if (g == NULL)
    {
        perror("Error en el shmat\n");
        exit(2);
    }

        // Gimme all the shared mem
    shmid = shmget(0x1234, sizeof(h), 0666 | IPC_CREAT);
    if (shmid == -1)
    {
        perror("Error en la memoria compartida\n");
        exit(1);
    }
    // Conectar la variable a la memoria compartida
    h = shmat(shmid, NULL, 0);
    if (h == NULL)
    {
        perror("Error en el shmat\n");
        exit(2);
    }
    *g = 0;
    *h = 0;
    srand(getpid());
    for (i = 0; i < 3; i++)
    {
        // Crea un nuevo proceso hijo que ejecuta la función proceso()
        pid = fork();
        if (pid == 0)
            proceso(i);
    }
    for (i = 0; i < 3; i++)
        pid = wait(&status);
    // See you, space cowboy
    shmdt(g);
    shmdt(h);
    shmdt(sem->blocked);
    shmdt(sem);
}