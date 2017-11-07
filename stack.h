#ifndef STACK_H_

#define STACK_H_

typedef struct stack_s stack_t;

struct stack_s {
  int top;
  int size;
  int* elements; 
};

// API

stack_t* stack_make(int size);

void     stack_push(stack_t* s, int v);

int      stack_empty(stack_t* s);

int      stack_pop(stack_t* s);

void     stack_free(stack_t* s);

#endif // STACK_H_
