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

  while (it->next->left != t->nil) it->next = it->next->left;

  return it;
}

int rb_tree_iter_has_next(rb_tree_iter_t* it) {
  return it->next != it->tree->nil;
}

rb_node_t* rb_tree_iter_next(rb_tree_iter_t* it) {
  rb_node_t* nil = it->tree->nil;
  rb_node_t* root = it->tree->root;
  rb_node_t* curr = it->next;
  rb_node_t* next = it->next;

  if (next->right != nil) {
    next = next->right;
    while (next->left != nil) {
      next = next->left;
    }
    it->next = next;
    return curr;
  }

  while (1) {
    if (next->parent == root) {
      it->next = nil;
      return curr;
    }
    if (next->parent->left == next) {
      next = next->parent;
      it->next = next;
      return curr;
    }
    next = next->parent;
  }
}

rb_tree_t* rb_tree_make(void) {
  rb_tree_t* n = malloc(sizeof(rb_tree_t));
  if (n == NULL) {
    perror("fatal: out of memory\n");
    exit(EXIT_FAILURE);
  }

  n->nil = malloc(sizeof(rb_node_t));
  if (n->nil == NULL) {
    perror("fatal: out of memory\n");
    exit(EXIT_FAILURE);
  }
  n->nil->parent = n->nil->left = n->nil->right = n->nil;
  n->nil->color = BLACK;
  n->nil->key = -1;

  n->root = malloc(sizeof(rb_node_t));
  if (n->root == NULL) {
    perror("fatal: out of memory\n");
    exit(EXIT_FAILURE);
  }
  n->root->parent = n->root->left = n->root->right = n->nil;
  n->root->color = BLACK;
  n->root->key = -1;

  return n;
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
  if (!t || !x) return;

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
  if (!t || !y) return;

  rb_node_t* x = y->left;
  y->left = x->right;

  if (x->right != t->nil) {
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
  if (!t || !z) return 1;

  int duplicate = 0;
  z->right = z->left = t->nil;
  rb_node_t* y = t->root;
  rb_node_t* x = t->root->left;

  while (x != t->nil) {
    y = x;
    if (x->key == z->key) {
      duplicate = 1;
      return duplicate;
    } else if (x->key > z->key) {
      x = x->left;
    } else {
      x = x->right;
    }
  }

  z->parent = y;

  if ((y == t->root) || z->key < y->key) {
    y->left = z;
  } else {
    y->right = z;
  }

  t->phobias[z->info - 1] += 1;

  return duplicate;
}

void rb_tree_insert(rb_tree_t* t, rb_node_t* x) {
  int result = rb_tree_insert_helper(t, x);
  if (result == 1) {
    free(x);
    x = NULL;
    return;
  }

  rb_node_t* y;
  rb_node_t* z = x;
  z->color = RED;

  while (z->parent->color == RED) {
    if (z->parent == z->parent->parent->left) {
      y = z->parent->parent->right;
      if (y->color == RED) {
	z->parent->color = BLACK;
	y->color = BLACK;
	z->parent->parent->color = RED;
	z = z->parent->parent;
      } else {
	if (z == z->parent->right) {
	  z = z->parent;
	  rb_tree_left_rotate(t, z);
	}
	z->parent->color = BLACK;
	z->parent->parent->color = RED;
	rb_tree_right_rotate(t, z->parent->parent);
      } 
    } else {
      y = z->parent->parent->left;
      if (y->color == RED) {
	z->parent->color = BLACK;
	y->color = BLACK;
	z->parent->parent->color = RED;
	z = z->parent->parent;
      } else {
	if (z == z->parent->left) {
	  z = z->parent;
	  rb_tree_right_rotate(t, z);
	}
	z->parent->color = BLACK;
	z->parent->parent->color = RED;
	rb_tree_left_rotate(t, z->parent->parent);
      } 
    }
  }

  t->root->left->color = BLACK;
}

int rb_tree_has(rb_tree_t* t, int k) {
  rb_node_t* x = t->root->left;

  while (x != t->nil) {
    if (x->key == k) {
      return 1;
    } else if (k < x->key) {
      x = x->left;
    } else {
      x = x->right;
    }
  }

  return 0;
}

void rb_tree_print_helper(rb_tree_t* t, rb_node_t* n) {
  if (n == t->nil) return;
  rb_tree_print_helper(t, n->left);
  printf("Key: %d\n", n->key+1);
  rb_tree_print_helper(t, n->right);
}

void rb_tree_print(rb_tree_t* t) {
  rb_tree_print_helper(t, t->root->left);
}

void rb_tree_merge(rb_tree_t* a, rb_tree_t* b) {
  rb_tree_iter_t* it = rb_tree_iter_make(b);

  while (rb_tree_iter_has_next(it)) {
    rb_node_t* c = rb_tree_iter_next(it);
    rb_tree_insert(a, rb_node_make(c->key, c->info));
  }

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

