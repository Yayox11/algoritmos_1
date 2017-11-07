#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "stack.h"
#include "rb_tree.h"

#define STACK_SIZE 128

int collapse_relationships(rb_tree_t** rels, int target) {
  if (!rels[target] || rels[target]->size == 1) return 0;

  rb_tree_t* t = rels[target];
  rb_tree_t* tmp = rb_tree_make();
  stack_t* s = stack_make(STACK_SIZE);

  stack_push(s, target);

  while (!stack_empty(s)) {
    int i = stack_pop(s);

    if (!rels[i]) continue;

    rb_tree_iter_t* iter = rb_tree_iter_make(rels[i]);
    while (rb_tree_iter_has_next(iter)) {
      rb_node_t* c = rb_tree_iter_next(iter);
      if (!rb_tree_has(tmp, c->key)) {
        rb_tree_insert(tmp, rb_node_make(c->key, c->info));
        stack_push(s, c->key);
      }
    }
    free(iter);
  }

  rb_tree_iter_t* iter = rb_tree_iter_make(tmp);
  while (rb_tree_iter_has_next(iter)) {
    rb_node_t* c = rb_tree_iter_next(iter);
    rb_tree_insert(t, rb_node_make(c->key, c->info));
    if (c->key != target) {
      rels[c->key] = NULL;
    }
  }
  free(iter);

  rb_tree_free(tmp);
  stack_free(s);

  return 1;
}

int main(int argc, char** argv) {
  int i, n, m, x, y;

  fscanf(stdin, "%d", &n);

  int* phobias = malloc(sizeof(int) * n);
  if (phobias == NULL) {
    perror("fatal: out of memory\n");
    exit(EXIT_FAILURE);
  }

  rb_tree_t** rels = malloc(sizeof(rb_tree_t*) * n);
  if (rels == NULL) {
    perror("fatal: out of memory\n");
    exit(EXIT_FAILURE);
  }

  for (i = 0; i < n; i++) {
    rels[i] = NULL;
  }

  // fill phobias array
  for (i = 0; i < n; i++) {
    int p;
    fscanf(stdin, "%d", &p);
    phobias[i] = p;
  }

  fscanf(stdin, "%d", &m);

  // fill relationships array
  for (i = 0; i < m; i++) {
    fscanf(stdin, "%d %d", &x, &y);

    if (x == y) continue;

    int p1 = --x;
    int p2 = --y;

    if (!rels[p1]) rels[p1] = rb_tree_make();
    if (!rels[p2]) rels[p2] = rb_tree_make();

    rb_tree_insert(rels[p1], rb_node_make(p2, phobias[p2]));
    rb_tree_insert(rels[p2], rb_node_make(p1, phobias[p1]));
  }

  int k = 0;
  for (i = 0; i < n; i++) {
    if (rels[i]) continue;
    rb_tree_t* t = rb_tree_make();
    rb_tree_insert(t, rb_node_make(i, phobias[i]));
    rels[i] = t;
    ++k;
  }

  for (i = 0; i < n; i++) {
    k += collapse_relationships(rels, i);
  }

  printf("%d\n", k);

  for (i = 0; i < n; i++) {
    if (rels[i] == NULL) continue;
    if (rels[i]->size == 1) {
      int idx = rels[i]->root->left->key;
      printf("%d\n", phobias[idx]);
    } else {
      int mcp = 0;
      int mcpc = 0;
      int j;
      for (j = 0; j < MAX_PHOBIAS; j++) {
        if (rels[i]->phobias[j] > mcpc) {
          mcp = j;
          mcpc = rels[i]->phobias[j];
        }
      }
      printf("%d\n", mcp+1);
    }
    rb_tree_free(rels[i]);
  }

  free(phobias);
  free(rels);

  exit(EXIT_SUCCESS);
}

