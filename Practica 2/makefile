all: 
	gcc -o init init.c 
	gcc -o getty getty.c
init: init.c
	gcc -o init init.c

getty: getty.c
	gcc -o getty getty.c

clean:
	rm getty init