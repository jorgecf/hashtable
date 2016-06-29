

all: lib tester

lib: 
	gcc -g -Wall -c srclib/hashtable.c
	ar rcs libhashtable.a hashtable.o


tester:
	gcc -g -Wall main.c -o main -L./ -lhashtable


clean:
	rm -f hashtable.o libhashtable.a main