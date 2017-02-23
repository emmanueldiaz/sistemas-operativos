all: exampleFCFS exampleRR exampleRR2 schedulerFCFS.o schedulerRR.o schedulerRR2.o libthreads.o

exampleFCFS: example.c libthreads.o schedulerFCFS.o
	gcc -o exampleFCFS example.c libthreads.o schedulerFCFS.o
	
exampleRR: example.c libthreads.o schedulerRR.o
	gcc -o exampleRR example.c libthreads.o schedulerRR.o
	
exampleRR2: example.c libthreads.o schedulerRR2.o
	gcc -o exampleRR2 example.c libthreads.o schedulerRR2.o
	
schedulerFCFS.o: schedulerFCFS.c libthreads.h commondata.h
	gcc -c schedulerFCFS.c
	
schedulerRR.o: schedulerRR.c libthreads.h commondata.h
	gcc -c schedulerRR.c

schedulerRR2.o: schedulerRR2.c libthreads.h commondata.h
	gcc -c schedulerRR2.c

libthreads.o: libthreads.c libthreads.h commondata.h
	gcc -c libthreads.c

clean:
	rm exampleFCFS exampleRR exampleRR2
	rm *.o