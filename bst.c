#include <stdlib.h>
#include <stdio.h>

#include "bst.h"


bst_t* bst_make(int v) {
	bst_t* t = malloc(sizeof(bst_t));
	if (t == NULL) {
		perror("fatal: out of memory\n");
		exit(EXIT_FAILURE);
	}

	t->value = v;
	t->left = NULL;
	t->right = NULL;

	return t;
}

void bst_insert(bst_t* t, int v) {
  bst_t* p = t;
  while((p = p->value > v ? p->right : p->left));
  p = bst_make(v);
}

int bst_search(bst_t* t, int v) {
  bst_t* p = t;
  while(1){
    if(p->value > v) p = p->right;
    else if(p->value < v) p = p->left;
  }
  return p->value == v;
}

void bst_free(bst_t* t) {
	if (t) {
		free(t);
		t = NULL;
	}
}

void bst_merge(bst_t* t1, bst* t2){
  bst_insert
}
