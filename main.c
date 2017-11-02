#include <stdlib.h>

typedef struct abb_s{
  int value;
  struct abb_s* left;
  struct abb_s* right;
}

int main(int argc, char** argv) {
  int n;

  int fobias[n];

  int m;

  bst_t* dict[n];

  int x,y;

  bst_insert(dict[x],y);
  bst_insert(dict[y],x);

  int i;
  for(i=0;i<n;i++){
    if(dict[i]){
      bst_t* p = dict[i];
      int* elements = bst_list(dict[i]);
      while (elements) {
        bst_merge(dict[i],dict[*elements]);
        bst_free(dict[*elements]);
        dict[*elements] = NULL;
        elements++;
      }
    }
  }

  exit(EXIT_SUCCESS);
}
