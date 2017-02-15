all: calcPI calcPI_clone calcPI_posix calcPI_fork

calcPI: calcPI.c
	gcc -o calcPI calcPI.c -lm

calcPI_clone: calcPI_clone.c
	gcc -o calcPI_clone calcPI_clone.c -lm

calcPI_posix: calcPI_posix.c
	gcc -o calcPI_posix calcPI_posix.c -pthread -lm

calcPI_fork: calcPI_fork.c
	gcc -o calcPI_fork calcPI_fork.c -lm

clean:
	rm calcPI calcPI_clone calcPI_posix calcPI_fork