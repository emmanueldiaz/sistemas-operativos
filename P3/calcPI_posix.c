#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#define NTHREADS 4
#define NITERATIONS 2000000000

long double total = 0; 
long double count[NTHREADS];

long double calculatePI(int lower, int higher);
void *threadPI(void *arg);

int main(int argc, char const *argv[]){

	pthread_t idthread[NTHREADS];
	int args[NTHREADS];

	clock_t begin = clock();

	//threads tomado de los ejemplos vistos en clase
	for(int i=0; i < NTHREADS; i++)
	{
		args[i]=i;
		pthread_create(&idthread[i],NULL,threadPI,(void *) &args[i]);
	}
	
	for(int i=0;i<NTHREADS;i++)
	{
		pthread_join(idthread[i],NULL);
		total+=count[i];
	}

	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("%f\n",time_spent);
	printf("%Lf\n",total);
	

	return 0;
}

long double calculatePI(int lower, int higher){

	long double fourthPI = 0;
    for (int i = lower; i < higher; ++i)
    {
        fourthPI+= (long double)pow(-1, i) / (2 * i + 1);
    }
    return 4*fourthPI;
}

void *threadPI(void *arg){
	int mynum= *((int *) arg);
	float threads = (float) NTHREADS;
	float base = (float) (1 / threads);
	int increase = (int) ceil(base * NITERATIONS);
	int tlower = 0, thigher = 0;
	
	if (mynum == 0){
		tlower = 0;
	}
	else{
		tlower = 0 + (mynum * increase) + 1;
	}

	if (mynum == NTHREADS -1){
		thigher = NITERATIONS;
	}
	else{
		thigher = 0 + ( (mynum + 1) * increase);
	}

	count[mynum] = calculatePI(tlower, thigher);
}