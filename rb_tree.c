#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "rb_tree.h"

rb_tree_iter_t* rb_tree_iter_make(rb_tree_t* t) {
  rb_tree_iter_t* it = malloc(sizeof(rb_tree_iter_t));
  if (it == NULL) {
    perror("fatal: out of memory\n");
    exit(EXIT_FAILURE);
  }

  it->tree = t;

  it->next = t->root;

  while (it->next->left != t->nil) {
    it->next = it->next->left;
  }

  return it;
}

rb_node_t* rb_tree_iter_next(rb_tree_iter_t* it) {
  if (it->next == NULL) {
    return NULL;
  }

  rb_node_t* r = it->next;
  rb_node_t* curr = it->next;

  if (curr->right != it->tree->nil) {
    curr = curr->right;
    while (curr->left != it->tree->nil && curr != curr->left) {
      printf("AAA\n");
      curr = curr->left;
    }
    it->next = curr;
    return r;
  }

  while (1) {
    printf("BBB\n");
    if (curr->parent == it->tree->root || curr == curr->parent) {
      printf("ROOT\n");
      it->next = NULL;
      return r;
    }
    if (curr->parent->left == curr) {
      curr = curr->parent;
      it->next = curr;
      return r;
    }
    curr = curr->parent;
  }
}

rb_tree_t* rb_tree_make(void) {
  rb_tree_t* new = malloc(sizeof(rb_tree_t));
  if (new == NULL) {
    perror("fatal: out of memory\n");
    exit(EXIT_FAILURE);
  }

  new->nil = malloc(sizeof(rb_node_t));
  if (new->nil == NULL) {
    perror("fatal: out of memory\n");
    exit(EXIT_FAILURE);
  }
  new->nil->parent = new->nil->left = new->nil->right = new->nil;
  new->nil->color = BLACK;
  new->nil->key = -1;

  new->root = malloc(sizeof(rb_node_t));
  if (new->root == NULL) {
    perror("fatal: out of memory\n");
    exit(EXIT_FAILURE);
  }
  new->root->parent = new->root->left = new->root->right = new->nil;
  new->root->color = BLACK;
  new->root->key = -1;

  new->size = 0;

  return new;
}

rb_node_t* rb_node_make(int key, int info) {
  rb_node_t* n = malloc(sizeof(rb_node_t));
  if (n == NULL) {
    perror("fatal: out of memory\n");
    exit(EXIT_FAILURE);
  }

  n->key = key;
  n->info = info;

  return n;
}

void rb_tree_left_rotate(rb_tree_t* t, rb_node_t* x) {
  rb_node_t* y = x->right;
  x->right = y->left;

  if (y->left != t->nil) {
    y->left->parent = x;
  }

  y->parent = x->parent;

  if (x == x->parent->left) {
    x->parent->left = y;
  } else {
    x->parent->right = y;
  }

  y->left = x;
  x->parent = y;
}

void rb_tree_right_rotate(rb_tree_t* t, rb_node_t* y) {
  rb_node_t* x = y->left;
  y->left = x->right;

  if (t->nil != x->right) {
    x->right->parent = y;
  }

  x->parent = y->parent;

  if (y == y->parent->left) {
    y->parent->left = x;
  } else {
    y->parent->right = x;
  }

  x->right = y;
  y->parent = x;
}

int rb_tree_insert_helper(rb_tree_t* t, rb_node_t* z) {
  z->left = z->right = t->nil;
  rb_node_t* y = t->root;
  rb_node_t* x = t->root->left;

  while (x != t->nil) {
    y = x;
    if (x->key > z->key) {
      x = x->left;
    } else {
      x = x->right;
    }
  }

  z->parent = y;

  if ((y == t->root) || (y->key > z->key)) {
    y->left = z;
  } else {
    if (y->key < z->key) {
      y->right = z;
    } else {
      return 1;
    }
  }

  t->size += 1;

  return 0;
}

void rb_tree_insert(rb_tree_t* t, rb_node_t* x) {
  int result = rb_tree_insert_helper(t, x);
  if (result != 0) {
    return;
  }

  x->color = RED;

  rb_node_t* y;

  while (x->parent->color == RED) {
    if (x->parent == x->parent->parent->left) {
      y = x->parent->parent->right;
      if (y->color == RED) {
	x->parent->color = BLACK;
	y->color = BLACK;
	x->parent->parent->color = RED;
	x = x->parent->parent;
      } else {
	if (x == x->parent->right) {
	  x = x->parent;
	  rb_tree_left_rotate(t, x);
	}
	x->parent->color = BLACK;
	x->parent->parent->color = RED;
	rb_tree_right_rotate(t, x->parent->parent);
      } 
    } else {
      y = x->parent->parent->left;
      if (y->color == RED) {
	x->parent->color = BLACK;
	y->color = BLACK;
	x->parent->parent->color = RED;
	x = x->parent->parent;
      } else {
	if (x == x->parent->left) {
	  x = x->parent;
	  rb_tree_right_rotate(t, x);
	}
	x->parent->color = BLACK;
	x->parent->parent->color = RED;
	rb_tree_left_rotate(t, x->parent->parent);
      } 
    }
  }

  t->root->left->color = BLACK;
}

rb_node_t* rb_tree_min(rb_tree_t* t) {
  rb_node_t* n = t->root->left;
  while (n->left != t->nil) n = n->left;
  return n;
}

rb_node_t* rb_tree_max(rb_tree_t* t) {
  rb_node_t* n = t->root->left;
  while (n->right != t->nil) n = n->right;
  return n;
}

unsigned int rb_tree_black_height(rb_tree_t* t) {
  unsigned int bh = 0;

  rb_node_t* n = t->root->left;

  while (n != t->nil) {
    if (n->color == BLACK) ++bh;
    n = n->left;
  }

  return bh;
}

void rb_tree_print_helper(rb_tree_t* t, rb_node_t* n) {
  if (n == t->nil) {
    return;
  }
  rb_tree_print_helper(t, n->left);
  printf("Person: %d\n", n->key+1);
  rb_tree_print_helper(t, n->right);
}

void rb_tree_print(rb_tree_t* t) {
  rb_tree_print_helper(t, t->root->left);
}

void rb_tree_merge_helper(rb_tree_t* t, rb_tree_t* t2, rb_node_t* n) {
  rb_tree_insert(t, n);
  if (n->left != t2->nil) rb_tree_merge_helper(t, t2, n->left);
  if (n->right != t2->nil) rb_tree_merge_helper(t, t2, n->right);
}

void rb_tree_merge(rb_tree_t* t1, rb_tree_t* t2) {
  rb_tree_iter_t* it = rb_tree_iter_make(t2);
  rb_node_t* c;
  while ((c = rb_tree_iter_next(it))) {
    if (c->key == -1) {
      break;
    }
    printf("C: key=%d\n", c->key+1);
    printf("Insertando persona %d\n", c->key+1);
    rb_tree_insert(t1, c);
    printf("FINAL\n");
  }
  printf("fuera del puto while\n");
  free(it);
}

void rb_tree_free_helper(rb_tree_t* t, rb_node_t* x) {
  if (x != t->nil) {
    rb_tree_free_helper(t, x->left);
    rb_tree_free_helper(t, x->right);
    free(x);
  }
}

void rb_tree_free(rb_tree_t* t) {
  if (t) {
    rb_tree_free_helper(t, t->root->left);
    free(t->root);
    free(t->nil);
    free(t);
    t = NULL;
  }
}
