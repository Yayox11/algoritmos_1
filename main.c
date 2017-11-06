#include <stdlib.h>
#include <stdio.h>

#include "rb_tree.h"

int main(int argc, char** argv) {
  int i, n, m, x, y;

  // read n
  fscanf(stdin, "%d", &n);

  int phobias[n];

  rb_tree_t* rels[n];
  for (i = 0; i < n; i++) rels[i] = NULL;

  // fill phobias array
  for (i = 0; i < n; i++) {
    int f;
    fscanf(stdin, "%d", &f);
    phobias[i] = f;
  }

  // read m
  fscanf(stdin, "%d", &m);

  // fill rels array
  for (i = 0; i < m; i++) {
    fscanf(stdin, "%d %d", &x, &y);

    if (x == y) continue;

    int p1 = --x;
    int p2 = --y;

    if (!rels[p1]) rels[p1] = rb_tree_make();

    rb_node_t* pn = rb_node_make(p2, phobias[p2]);
    rb_tree_insert(rels[p1], pn);
  }

  for (i = 0; i < n; i++) {
    if (rels[i] == NULL) continue;
    printf("Arbol persona %d\n", i+1);
    rb_tree_print(rels[i]);
    printf("\n");
  }

  printf("!!!!!!!!!!!!!MERGE!!!!!!!!!!!!!!!!\n");

  for (i = 0; i < n; i++) {
    printf("Relaciones de la persona %d\n", i+1);
    if (rels[i] == NULL) continue;

    rb_tree_t* t = rels[i];
    rb_tree_iter_t* it = rb_tree_iter_make(t);

    rb_node_t* c;
    while ((c = rb_tree_iter_next(it))) {
      if (rels[c->key]) {
        printf("c: person=%d\n", c->key+1);
        printf("Mezclando\n");
        rb_tree_t* b = rels[c->key];
        rb_tree_print(b);
        printf("Con\n");
        rb_tree_print(t);
        // merge inline
        /*
        rb_tree_iter_t* it2 = rb_tree_iter_make(b);
        rb_node_t* c2;
        while ((c2 = rb_tree_iter_next(it))) {
          printf("Insertando persona %d\n", c2->key+1);
          rb_tree_insert(t, c2);
          printf("Persona %d insertada\n", c2->key+1);
        }
        printf("fuera del puto while\n");
        free(it2);
        */
        rb_tree_merge(t, b);
        printf("Nuevas relaciones de la persona %d\n", i+1);
        rb_tree_print(t);
        printf("Listo\n");
        //rb_tree_free(b);
        rels[c->key] = NULL;
      }
    }

    free(it);

    //rb_node_t* last = rb_node_make(i, phobias[i]);
    //rb_tree_insert(t, last);
  }

  for (i = 0; i < n; i++) {
    if (rels[i] == NULL) continue;
    printf("Arbol persona %d\n", i+1);
    rb_tree_print(rels[i]);
    printf("\n");
  }

  exit(EXIT_SUCCESS);
}

