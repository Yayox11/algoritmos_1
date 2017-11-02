#ifndef BST_H_

#define BST_H_

typedef struct bst_s{
  int value;
  struct bst_s* left;
  struct bst_s* right;
}

// API

bst_t* bst_make();
void bst_insert(bst_t* t, int v);
int bst_search(bst_t* t, int v);
void bst_free(bst_t* t);
void bst_merge(bst_t* t1, bst_t* t2);

#endif // BST_H_
