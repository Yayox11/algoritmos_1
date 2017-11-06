all: rb_tree.o tarea1 

rb_tree.o: rb_tree.c
	gcc -c rb_tree.c

tarea1: rb_tree.h rb_tree.o main.c
	mkdir -p bin
	gcc -Wall -o bin/tarea1 rb_tree.o main.c

clean:
	rm -rf bin *.o
