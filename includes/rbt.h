// based on sedgewick's paper
#ifndef RBT_H
#define RBT_H
#include <stdarg.h>
#include <stdlib.h>
#ifndef BST_H
#define RED_COLOR 1
#define BLACK_COLOR 0
#define RedBlackTree(KEY, VALUE, IS_LESS, IS_MORE) \
typedef struct KEY ## _ ## VALUE ## _rbt * KEY ## _ ## VALUE ## _rbt; \
typedef struct List_ ## KEY ## _ ## VALUE ## _pair *List_ ## KEY ## _ ## VALUE ## _pair; \
typedef struct KEY ## _ ## VALUE ## _pair * KEY ## _ ## VALUE ## _pair; \
struct KEY ## _ ## VALUE ## _pair { \
  /* basic pair structure to hold key and value and return from find() just like std::pair */ \
  KEY key; \
  VALUE value; \
}; \
struct List_ ## KEY ## _ ## VALUE ## _pair { \
  /* a list structure basically for inorder traversal, contains the key-value pair as entry and pointer to next */ \
  KEY ## _ ## VALUE ## _pair entry; \
  List_ ## KEY ## _ ## VALUE ## _pair next; \
}; \
\
List_ ## KEY ## _ ## VALUE ## _pair make_list_ ## KEY ## _ ## VALUE ## _(KEY key, VALUE value){ \
  List_ ## KEY ## _ ## VALUE ## _pair p_list = malloc(sizeof(struct List_ ## KEY ## _ ## VALUE ## _pair)); \
  /* each list node keeps its own copy of the 'pair' so that when freeing the the List (ie. all the list nodes) we don't have double freeing */ \
  /* later on, since the same pair would be shared by the RBT node */ \
  /* so when passing the key and value to create a list node, they also should ideally be not shared if they are non primitive types */ \
  /* if Allaah Wills, maybe in future we will have the opportunity to pass some sort of a copy constructor too */ \
  /* as of now, double freeing is possible since the Key and value would be shared between list node and rbt node */ \
  /* another approach is to free the (inorder traversal) list first and then free the RBT, using a kind of a "shallow delete" for the \
   key and value types in RBT's destruct function, so that it only deletes the pair struct and the rbt node and not the key and value */ \
  KEY ## _ ## VALUE ## _pair p_entry = malloc(sizeof(struct KEY ## _ ## VALUE ## _pair)); \
  p_entry->key = key; \
  p_entry->value = value; \
  p_list->entry = p_entry; \
  p_list->next = NULL; \
  return p_list; \
} \
void shallow_destroy_List_ ## KEY ## _ ## VALUE ## _rbt(List_ ## KEY ## _ ## VALUE ## _pair list, void (*shallow_destruct) (KEY ## _ ## VALUE ## _pair)) { \
  /* does not destroy the list-node's entry's key-value, only destroys the list and entry object */ \
  /* need to rename this function (remove the _rbt from the end of function name) and add the same to bst.h */ \
  if (!list) return; \
  List_ ## KEY ## _ ## VALUE ## _pair temp_list_node = list->next; \
  /* destruct(list->entry); */ \
  shallow_destruct(list->entry); \
  free(list); \
  list = temp_list_node; \
  shallow_destroy_List_ ## KEY ## _ ## VALUE ## _rbt(list, shallow_destruct); \
} \
void deep_destroy_List_ ## KEY ## _ ## VALUE ## _rbt(List_ ## KEY ## _ ## VALUE ## _pair list, void (*destruct) (KEY ## _ ## VALUE ## _pair)) { \
  if (!list) return; \
  List_ ## KEY ## _ ## VALUE ## _pair temp_list_node = list->next; \
  destruct(list->entry); \
  free(list); \
  list = temp_list_node; \
  deep_destroy_List_ ## KEY ## _ ## VALUE ## _rbt(list, destruct); \
} \
\
List_ ## KEY ## _ ## VALUE ## _pair last_ ## KEY ## _ ## VALUE ## _(List_ ## KEY ## _ ## VALUE ## _pair lst) { \
  /* finds the last element of a list */ \
  if (!lst) return NULL; /* only matters the first time */ \
  if (lst->next == NULL) return lst; \
  /* otherwise recurse */ \
  return last_ ## KEY ## _ ## VALUE ## _(lst->next); \
} \
List_ ## KEY ## _ ## VALUE ## _pair append_List_ ## KEY ## _ ## VALUE ## _rbt(int numLists, ...) { \
  /* useful for appending sublists in inorder method, uses varargs */ \
  va_list valst; \
  va_start(valst, numLists);\
  List_ ## KEY ## _ ## VALUE ## _pair temp_list, full_list; \
  temp_list = va_arg(valst, List_ ## KEY ## _ ## VALUE ## _pair); \
  int count = 1; \
  while (!temp_list && count <= numLists) { \
    /* remove all NULL lists (like '() in scheme is discarded in (append ..)) */ \
    temp_list = va_arg(valst, List_ ## KEY ## _ ## VALUE ## _pair); \
    count++; \
  } \
  full_list = temp_list; /* saving a reference to the first list, all other lists will be concatenated at their ends */ \
  temp_list = last_ ## KEY ## _ ## VALUE ## _(temp_list); \
  for (int i = count; i < numLists; i++) { \
    temp_list->next = va_arg(valst, List_ ## KEY ## _ ## VALUE ## _pair); \
    temp_list = last_ ## KEY ## _ ## VALUE ## _(temp_list); \
  } \
  return full_list;\
} \
struct KEY ## _ ## VALUE ## _rbt { \
  KEY ## _ ## VALUE ## _pair value_type; \
  KEY ## _ ## VALUE ## _rbt left; \
  KEY ## _ ## VALUE ## _rbt right; \
  int size; \
  int color; /* 1 for Red, 0 for Black */ \
}; \
KEY ## _ ## VALUE ## _rbt make_ ## KEY ## _ ## VALUE ## _rbt(KEY key, VALUE value, int size, int color) { \
  KEY ## _ ## VALUE ## _rbt p_rbt = malloc(sizeof(struct KEY ## _ ## VALUE ## _rbt)); \
  KEY ## _ ## VALUE ## _pair p_pair = malloc(sizeof(struct KEY ## _ ## VALUE ## _pair)); \
  p_pair->key = key; \
  p_pair->value = value; \
  p_rbt->value_type = p_pair; \
  p_rbt->left = NULL; \
  p_rbt->right = NULL; \
  p_rbt->size = size; \
  p_rbt->color = color; \
  return p_rbt; \
} \
void free_ ## KEY ## _ ## VALUE ## _rbt(KEY ## _ ## VALUE ## _rbt x) { \
  free(x); \
} \
int size_ ## KEY ## _ ## VALUE ## _rbt(KEY ## _ ## VALUE ## _rbt x) { \
  if (!x) return 0; \
  return x->size; \
} \
KEY ## _ ## VALUE ## _pair find_in_ ## KEY ## _ ## VALUE ## _rbt(KEY ## _ ## VALUE ## _rbt root, KEY key) { \
 if (!root) return NULL; \
 if (IS_LESS ## _(key, root->value_type->key)) return find_in_ ## KEY ## _ ## VALUE ## _rbt(root->left, key); \
 else if (IS_MORE ## _(key, root->value_type->key)) return find_in_ ## KEY ## _ ## VALUE ## _rbt(root->right, key); \
 else return root->value_type; \
} \
/* min is identical to min for vanilla BST */ \
KEY ## _ ## VALUE ## _pair min_ ## KEY ## _ ## VALUE ## _rbt(KEY ## _ ## VALUE ## _rbt root) { \
  /* return minimum key-value pair */ \
  if (!root) return NULL; \
  if (!(root->left)) return root->value_type; \
  return min_ ## KEY ## _ ## VALUE ## _rbt(root->left); \
} \
KEY ## _ ## VALUE ## _rbt min_node_ ## KEY ## _ ## VALUE ## _rbt(KEY ## _ ## VALUE ## _rbt root) { \
  /* return minimum rbt node, useful in deletion */ \
  if (!root) return NULL; \
  if (!(root->left)) return root; \
  return min_node_ ## KEY ## _ ## VALUE ## _rbt(root->left); \
} \
KEY ## _ ## VALUE ## _pair max_ ## KEY ## _ ## VALUE ## _rbt(KEY ## _ ## VALUE ## _rbt root) { \
  /* return maximum key-value pair */ \
  if (!root) return NULL; \
  if (!(root->right)) return root->value_type; \
  return max_ ## KEY ## _ ## VALUE ## _rbt(root->right); \
} \
int isRed_ ## KEY ## _ ## VALUE ## _rbt(KEY ## _ ## VALUE ## _rbt x) { \
  if (!x) return 0; \
  return x->color == RED_COLOR; \
} \
KEY ## _ ## VALUE ## _rbt rotate_left_ ## KEY ## _ ## VALUE ## _(KEY ## _ ## VALUE ## _rbt h) { \
  KEY ## _ ## VALUE ## _rbt x = h->right; \
  h->right = x->left; \
  x->left = h; \
  x->color = h->color; \
  h->color = RED_COLOR; \
  x->size = h->size; \
  h->size = 1 + size_ ## KEY ## _ ## VALUE ## _rbt(h->left) + size_ ## KEY ## _ ## VALUE ## _rbt(h->right); \
  return x; \
  } \
KEY ## _ ## VALUE ## _rbt rotate_right_ ## KEY ## _ ## VALUE ## _(KEY ## _ ## VALUE ## _rbt h) { \
  KEY ## _ ## VALUE ## _rbt x = h->left; \
  h->left = x->right; \
  x->right = h; \
  x->color = h->color; \
  h->color = RED_COLOR; \
  x->size = h->size; \
  h->size = 1 + size_ ## KEY ## _ ## VALUE ## _rbt(h->left) + size_ ## KEY ## _ ## VALUE ## _rbt(h->right); \
  return x; \
} \
void flip_colors_ ## KEY ## _ ## VALUE ## _(KEY ## _ ## VALUE ## _rbt h) { \
  h->color = RED_COLOR; \
  h->left->color = BLACK_COLOR; \
  h->right->color = BLACK_COLOR; \
} \
KEY ## _ ## VALUE ## _rbt insert_helper_ ## KEY ## _ ## VALUE ## _rbt(KEY ## _ ## VALUE ## _rbt root, KEY key, VALUE value) { \
  if (!root) return make_ ## KEY ## _ ## VALUE ## _rbt(key, value, 1, RED_COLOR); \
  if (IS_LESS ## _(key, root->value_type->key)) root->left = insert_helper_ ## KEY ## _ ## VALUE ## _rbt(root->left, key, value); \
  else if (IS_MORE ## _(key, root->value_type->key)) root->right = insert_helper_ ## KEY ## _ ## VALUE ## _rbt(root->right, key, value); \
  else root->value_type->value = value; \
  if (isRed_ ## KEY ## _ ## VALUE ## _rbt(root->right) && !isRed_ ## KEY ## _ ## VALUE ## _rbt(root->left)) root = rotate_left_ ## KEY ## _ ## VALUE ## _(root); \
  if (isRed_ ## KEY ## _ ## VALUE ## _rbt(root->left) && isRed_ ## KEY ## _ ## VALUE ## _rbt(root->left->left)) root = rotate_right_ ## KEY ## _ ## VALUE ## _(root); \
  if (isRed_ ## KEY ## _ ## VALUE ## _rbt(root->left) && isRed_ ## KEY ## _ ## VALUE ## _rbt(root->right)) flip_colors_ ## KEY ## _ ## VALUE ## _(root); \
  root->size = 1 + size_ ## KEY ## _ ## VALUE ## _rbt(root->left) + size_ ## KEY ## _ ## VALUE ## _rbt(root->right); \
  return root; \
} \
KEY ## _ ## VALUE ## _rbt insert_ ## KEY ## _ ## VALUE ## _rbt(KEY ## _ ## VALUE ## _rbt root, KEY key, VALUE value) { \
  root = insert_helper_ ## KEY ## _ ## VALUE ## _rbt(root, key, value); \
  root->color = BLACK_COLOR; \
  return root; \
} \
/* deletion procedures in RBTs */ \
/* sedgewick used 'h' as the node parameter */ \
void flip_colors_gen_ ## KEY ## _ ## VALUE ## _rbt(KEY ## _ ## VALUE ## _rbt h) { \
  h->color = !h->color; \
  h->left->color = !h->left->color; \
  h->right->color = !h->right->color; \
} \
KEY ## _ ## VALUE ## _rbt move_red_left_ ## KEY ## _ ## VALUE ## _rbt(KEY ## _ ## VALUE ## _rbt h) { \
  flip_colors_gen_ ## KEY ## _ ## VALUE ## _rbt(h); \
  if (isRed_ ## KEY ## _ ## VALUE ## _rbt(h->right->left)) { \
    h->right = rotate_right_ ## KEY ## _ ## VALUE ## _(h->right); \
    h = rotate_left_ ## KEY ## _ ## VALUE ## _(h); \
    flip_colors_gen_ ## KEY ## _ ## VALUE ## _rbt(h); \
  } \
  return h; \
} \
KEY ## _ ## VALUE ## _rbt move_red_right_ ## KEY ## _ ## VALUE ## _rbt(KEY ## _ ## VALUE ## _rbt h) { \
  flip_colors_gen_ ## KEY ## _ ## VALUE ## _rbt(h); \
  if (isRed_ ## KEY ## _ ## VALUE ## _rbt(h->left->left)) { \
    h = rotate_right_ ## KEY ## _ ## VALUE ## _(h);\
    flip_colors_gen_ ## KEY ## _ ## VALUE ## _rbt(h); \
  }\
  return h; \
  } \
KEY ## _ ## VALUE ## _rbt balance_ ## KEY ## _ ## VALUE ## _rbt(KEY ## _ ## VALUE ## _rbt h) { \
  if (isRed_ ## KEY ## _ ## VALUE ## _rbt(h->right)) h = rotate_left_ ## KEY ## _ ## VALUE ## _(h); \
  if (isRed_ ## KEY ## _ ## VALUE ## _rbt(h->left) && isRed_ ## KEY ## _ ## VALUE ## _rbt(h->left->left)) h = rotate_right_ ## KEY ## _ ## VALUE ## _(h); \
  if (isRed_ ## KEY ## _ ## VALUE ## _rbt(h->left) && isRed_ ## KEY ## _ ## VALUE ## _rbt(h->right)) flip_colors_gen_ ## KEY ## _ ## VALUE ## _rbt(h);  \
  h->size = 1 + size_ ## KEY ## _ ## VALUE ## _rbt(h->left) + size_ ## KEY ## _ ## VALUE ## _rbt(h->right); \
  return h; \
 } \
KEY ## _ ## VALUE ## _rbt delete_min_helper_ ## KEY ## _ ## VALUE ## _rbt(KEY ## _ ## VALUE ## _rbt h, void (* destruct) (KEY ## _ ## VALUE ## _pair)) { \
  if (!h->left) { \
    if (h->value_type) { \
      destruct(h->value_type);\
    } \
    \
    free(h); \
    return NULL; \
  } \
  if (!isRed_ ## KEY ## _ ## VALUE ## _rbt(h->left) && !isRed_ ## KEY ## _ ## VALUE ## _rbt(h->left->left)) h = move_red_left_ ## KEY ## _ ## VALUE ## _rbt(h); \
  h->left = delete_min_helper_ ## KEY ## _ ## VALUE ## _rbt(h->left, destruct); \
  return balance_ ## KEY ## _ ## VALUE ## _rbt(h); \
} \
KEY ## _ ## VALUE ## _rbt delete_min_ ## KEY ## _ ## VALUE ## _rbt(KEY ## _ ## VALUE ## _rbt root, void (* destruct) (KEY ## _ ## VALUE ## _pair)) { \
  if (!root) return NULL;  /* for the case where the tree is empty */ \
  /* below line is in the book (algs 4) but absent from sedgewick paper https://www.cs.princeton.edu/~rs/talks/LLRB/LLRB.pdf */ \
  if (!isRed_ ## KEY ## _ ## VALUE ## _rbt(root->left) && !isRed_ ## KEY ## _ ## VALUE ## _rbt(root->right)) root->color = RED_COLOR; \
  root = delete_min_helper_ ## KEY ## _ ## VALUE ## _rbt(root, destruct); \
  if (root) root->color = BLACK_COLOR; \
  return root; \
} \
KEY ## _ ## VALUE ## _rbt delete_max_helper_ ## KEY ## _ ## VALUE ## _rbt(KEY ## _ ## VALUE ## _rbt h, void (* destruct) (KEY ## _ ## VALUE ## _pair)) { \
  if (isRed_ ## KEY ## _ ## VALUE ## _rbt(h->left))  h = rotate_right_ ## KEY ## _ ## VALUE ## _(h);\
  if (h->right == NULL) { \
    destruct(h->value_type); \
    free(h); \
    return NULL; \
  } \
  if (!isRed_ ## KEY ## _ ## VALUE ## _rbt(h->right) && !isRed_ ## KEY ## _ ## VALUE ## _rbt(h->right->left)) h = move_red_right_ ## KEY ## _ ## VALUE ## _rbt(h); \
  h->right = delete_max_helper_ ## KEY ## _ ## VALUE ## _rbt(h->right, destruct); \
  return balance_ ## KEY ## _ ## VALUE ## _rbt(h); \
} \
KEY ## _ ## VALUE ## _rbt delete_max_ ## KEY ## _ ## VALUE ## _rbt(KEY ## _ ## VALUE ## _rbt root, void (* destruct) (KEY ## _ ## VALUE ## _pair)) { \
  if (!isRed_ ## KEY ## _ ## VALUE ## _rbt(root->left) && !isRed_ ## KEY ## _ ## VALUE ## _rbt(root->right)) root->color = RED_COLOR; \
  root = delete_max_helper_ ## KEY ## _ ## VALUE ## _rbt(root, destruct); \
  if (root) root->color = BLACK_COLOR; \
  return root; \
} \
KEY ## _ ## VALUE ## _rbt delete_helper_ ## KEY ## _ ## VALUE ## _rbt(KEY ## _ ## VALUE ## _rbt h, KEY key, void (* destruct) (KEY ## _ ## VALUE ## _pair)) { \
  if (IS_LESS ## _(key, h->value_type->key)) { \
    if (!isRed_ ## KEY ## _ ## VALUE ## _rbt(h->left) && !isRed_ ## KEY ## _ ## VALUE ## _rbt(h->left->left)) h = move_red_left_ ## KEY ## _ ## VALUE ## _rbt(h); \
    h->left = delete_helper_ ## KEY ## _ ## VALUE ## _rbt(h->left, key, destruct); \
  } \
  else {\
    if (isRed_ ## KEY ## _ ## VALUE ## _rbt(h->left)) h = rotate_right_ ## KEY ## _ ## VALUE ## _(h); \
    int is_l = IS_LESS ## _(key, h->value_type->key); \
    int is_m = IS_MORE ## _(key, h->value_type->key); \
    if (!is_l && !is_m && !h->right) { \
      destruct(h->value_type);\
      free(h); \
      return NULL; \
      } \
    if (!isRed_ ## KEY ## _ ## VALUE ## _rbt(h->right) && !isRed_ ## KEY ## _ ## VALUE ## _rbt(h->right->left)) h = move_red_right_ ## KEY ## _ ## VALUE ## _rbt(h); \
    \
    is_l = IS_LESS ## _(key, h->value_type->key); \
    is_m = IS_MORE ## _(key, h->value_type->key); \
    if (!is_l && !is_m) { \
      KEY ## _ ## VALUE ## _rbt x = min_node_ ## KEY ## _ ## VALUE ## _rbt(h->right); \
      h->value_type->key = x->value_type->key; \
      h->value_type->value = x->value_type->value; \
      h->right = delete_min_helper_ ## KEY ## _ ## VALUE ## _rbt(h->right, destruct); \
    } else h->right = delete_helper_ ## KEY ## _ ## VALUE ## _rbt(h->right, key, destruct); \
  } \
  return balance_ ## KEY ## _ ## VALUE ## _rbt(h); \
} \
KEY ## _ ## VALUE ## _rbt delete_ ## KEY ## _ ## VALUE ## _rbt(KEY ## _ ## VALUE ## _rbt root, KEY key, void (* destruct) (KEY ## _ ## VALUE ## _pair)) { \
  if (!isRed_ ## KEY ## _ ## VALUE ## _rbt(root->left) && !isRed_ ## KEY ## _ ## VALUE ## _rbt(root->right)) root->color = RED_COLOR; \
  root = delete_helper_ ## KEY ## _ ## VALUE ## _rbt(root, key, destruct); \
  if (root) root->color = BLACK_COLOR; \
  return root; \
} \
const List_ ## KEY ## _ ## VALUE ## _pair recursive_inorder_ ## KEY ## _ ## VALUE ## _rbt(KEY ## _ ## VALUE ## _rbt root) { \
  if (!root) return NULL; \
  return append_List_ ## KEY ## _ ## VALUE ## _rbt(3, recursive_inorder_ ## KEY ## _ ## VALUE ## _rbt(root->left), \
    make_list_ ## KEY ## _ ## VALUE ## _(root->value_type->key, root->value_type->value), \
    recursive_inorder_ ## KEY ## _ ## VALUE ## _rbt(root->right));\
} \
void helper_free_whole_rbt_ ## KEY ## _ ## VALUE ## _(KEY ## _ ## VALUE ## _rbt root, void (* destruct) (KEY ## _ ## VALUE ## _pair)) { \
  /* one way to do this */ \
  \
  /* while(size_ ## KEY ## _ ## VALUE ## _rbt(root)) { \
    \
    root = delete_min_ ## KEY ## _ ## VALUE ## _rbt(root, destruct); \
  } */ \
  \
  /* only very slightly faster */ \
  if (!root) return;  \
  \
  /* free  */ \
  helper_free_whole_rbt_ ## KEY ## _ ## VALUE ## _(root->left, destruct);\
  helper_free_whole_rbt_ ## KEY ## _ ## VALUE ## _(root->right, destruct);\
  if (root->value_type) { \
    destruct(root->value_type);\
  } \
  free(root);  \
} \
void free_whole_rbt_ ## KEY ## _ ## VALUE ## _(KEY ## _ ## VALUE ## _rbt *root, void (* destruct) (KEY ## _ ## VALUE ## _pair)) { \
  helper_free_whole_rbt_ ## KEY ## _ ## VALUE ## _(*root, destruct); \
  *root = NULL; \
}
#else
#define RedBlackTree(KEY, VALUE, IS_LESS, IS_MORE) int add(int a , int b) {return 0;}
#endif
#endif
