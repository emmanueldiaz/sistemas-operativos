#include <stdio.h>
#include <math.h>
#include <unistd.h>

#define NITERATIONS 100
//dos mil millones 2 000 000 000 tiempo base

int main(int argc, char const *argv[])
{
	long double fourthPI = 0;
	for (int i = 0; i < NITERATIONS; ++i)
	{
		 fourthPI+=(long double)pow(-1,i)/(2*i+1);
	}

	printf("%Lf\n",4*fourthPI);
	return 0;
}