// Red-black tree implementation

#ifndef RB_TREE_H_

#define RB_TREE_H_

typedef enum rb_color_e rb_color_t;

enum rb_color_e {
  RED,
  BLACK
};

typedef struct rb_node_s rb_node_t;

struct rb_node_s {
  int key;  // person index
  int info; // phobia index
  rb_color_t color;
  rb_node_t* left;
  rb_node_t* right;
  rb_node_t* parent;
};

typedef struct rb_tree_s rb_tree_t;

struct rb_tree_s {
  int size;
  rb_node_t* root;
  rb_node_t* nil;
};

typedef struct rb_tree_iter_s rb_tree_iter_t;

struct rb_tree_iter_s {
  rb_tree_t* tree;
  rb_node_t* next;
};

// iter API

rb_tree_iter_t* rb_tree_iter_make(rb_tree_t* t);

rb_node_t* rb_tree_iter_next(rb_tree_iter_t* it);

// API

rb_tree_t* rb_tree_make(void);

rb_node_t* rb_node_make(int key, int info);

void       rb_tree_merge(rb_tree_t* t1, rb_tree_t* t2);

rb_node_t* rb_tree_min(rb_tree_t* t);

rb_node_t* rb_tree_max(rb_tree_t* t);

rb_node_t* rb_tree_next(rb_tree_t* t);

void       rb_tree_insert(rb_tree_t* t, rb_node_t* z);

void       rb_tree_print(rb_tree_t* t);

void       rb_tree_free(rb_tree_t* t);

#endif // RB_TREE_H_

