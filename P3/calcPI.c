#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <time.h>

#define NITERATIONS 2000000000
//dos mil millones 2 000 000 000 tiempo base

int main(int argc, char const *argv[])
{
	long double fourthPI = 0;
	clock_t begin = clock();
	for (int i = 0; i < NITERATIONS; ++i)
	{
		 fourthPI+=(long double)pow(-1,i)/(2*i+1);
	}
	clock_t end = clock();
	printf("%Lf\n",4*fourthPI);
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("%f\n",time_spent);
	return 0;
}