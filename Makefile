all: stack.o rb_tree.o tarea1 

stack.o: stack.c
	gcc -c stack.c

rb_tree.o: rb_tree.c
	gcc -c rb_tree.c

tarea1: stack.h stack.o rb_tree.h rb_tree.o main.c
	mkdir -p bin
	gcc -Wall -o bin/tarea1 stack.o rb_tree.o main.c

clean:
	rm -rf bin *.o
