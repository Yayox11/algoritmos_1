#include <stdlib.h>
#include <stdio.h>

#include "stack.h"

stack_t* stack_make(int size) {
  if (size < 1) {
    perror("fatal: attempt to make stack of size < 1\n");
    exit(EXIT_FAILURE);
  }

  stack_t* s = malloc(sizeof(stack_t));
  if (s == NULL) {
    perror("fatal: out of memory\n");
    exit(EXIT_FAILURE);
  }

  s->top = -1;
  s->size = size;

  s->elements = malloc(sizeof(int) * size);
  if (s->elements == NULL) {
    perror("fatal: out of memory\n");
    exit(EXIT_FAILURE);
  }

  return s;
}

void stack_grow(stack_t* s) {
  int* nels = malloc(sizeof(int) * s->size * 2);
  if (nels == NULL) {
    perror("fatal: out of memory\n");
    exit(EXIT_FAILURE);
  }

  int i;
  for (i = 0; i < s->size; i++) {
    nels[i] = s->elements[i];
  }

  free(s->elements);
  s->elements = nels;
}

int stack_empty(stack_t* s) {
  return s->top == -1;
}

void stack_push(stack_t* s, int v) {
  if (s->top == s->size - 1) {
    stack_grow(s);
  }

  s->elements[++s->top] = v;
}

int stack_pop(stack_t* s) {
  if (s->top == -1) {
    perror("stack empty\n");
    exit(EXIT_FAILURE);
  };

  return s->elements[s->top--];
}

void stack_free(stack_t* s) {
  if (s) {
    free(s->elements);
    free(s);
    s = NULL;
  }
}

