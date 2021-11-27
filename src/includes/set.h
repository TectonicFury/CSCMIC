#ifndef SET_H
#define SET_H
#include <stdarg.h>
#include <stdlib.h>
#define RED_COLOR 1
#define BLACK_COLOR 0
#define SET(KEY, IS_LESS,IS_MORE) \
typedef struct KEY ## _set * KEY ## _set; \
typedef struct List_ ## KEY ## _set_entry *List_ ## KEY ## _set_entry; \
typedef struct KEY ## _set_entry * KEY ## _set_entry; \
struct KEY ## _set_entry { \
  /* basic set entry structure to hold key */ \
  KEY key; \
}; \
struct List_ ## KEY ## _set_entry { \
  /* a list structure basically for inorder traversal, contains the key-value pair as entry and pointer to next */ \
  KEY ## _set_entry entry; \
  List_ ## KEY ## _set_entry next; \
}; \
\
List_ ## KEY ## _set_entry make_list_ ## KEY ## _set(KEY key){ \
  List_ ## KEY ## _set_entry p_list = malloc(sizeof(struct List_ ## KEY ## _set_entry)); \
  KEY ## _set_entry p_entry = malloc(sizeof(struct KEY ## _set_entry)); \
  p_entry->key = key; \
  p_list->entry = p_entry; \
  p_list->next = NULL; \
  return p_list; \
} \
void shallow_destroy_List_ ## KEY ## _set(List_ ## KEY ## _set_entry list, void (*shallow_destruct) (KEY ## _set_entry)) { \
  /* does not destroy the list-node's entry's key-value, only destroys the list and entry object */ \
  /* need to rename this function (remove the _rbt from the end of function name) and add the same to bst.h */ \
  if (!list) return; \
  List_ ## KEY ## _set_entry temp_list_node = list->next; \
  /* destruct(list->entry); */ \
  shallow_destruct(list->entry); \
  free(list); \
  list = temp_list_node; \
  shallow_destroy_List_ ## KEY ## _set(list, shallow_destruct); \
} \
void deep_destroy_List_ ## KEY ## _set_entry(List_ ## KEY ## _set_entry list, void (*destruct) (KEY ## _set_entry)) { \
  if (!list) return; \
  List_ ## KEY ## _set_entry temp_list_node = list->next; \
  destruct(list->entry); \
  free(list); \
  list = temp_list_node; \
  deep_destroy_List_ ## KEY ## _set_entry(list, destruct); \
} \
\
List_ ## KEY ## _set_entry last_ ## KEY ## _set_entry(List_ ## KEY ## _set_entry lst) { \
  /* finds the last element of a list */ \
  if (!lst) return NULL; /* only matters the first time */ \
  if (lst->next == NULL) return lst; \
  /* otherwise recurse */ \
  return last_ ## KEY ## _set_entry(lst->next); \
} \
List_ ## KEY ## _set_entry append_List_ ## KEY ## _set_entry(int numLists, ...) { \
  /* useful for appending sublists in inorder method, uses varargs */ \
  va_list valst; \
  va_start(valst, numLists);\
  List_ ## KEY ## _set_entry temp_list, full_list; \
  temp_list = va_arg(valst, List_ ## KEY ## _set_entry); \
  int count = 1; \
  while (!temp_list && count <= numLists) { \
    /* remove all NULL lists (like '() in scheme is discarded in (append ..)) */ \
    temp_list = va_arg(valst, List_ ## KEY ## _set_entry); \
    count++; \
  } \
  full_list = temp_list; /* saving a reference to the first list, all other lists will be concatenated at their ends */ \
  temp_list = last_ ## KEY ## _set_entry(temp_list); \
  for (int i = count; i < numLists; i++) { \
    temp_list->next = va_arg(valst, List_ ## KEY ## _set_entry); \
    temp_list = last_ ## KEY ## _set_entry(temp_list); \
  } \
  return full_list;\
} \
struct KEY ## _set { \
  KEY ## _set_entry value_type; \
  KEY ## _set left; \
  KEY ## _set right; \
  int size; \
  int color; /* 1 for Red, 0 for Black */ \
}; \
KEY ## _set make_ ## KEY ## _set(KEY key, int size, int color) { \
  KEY ## _set p_set = malloc(sizeof(struct KEY ## _set)); \
  KEY ## _set_entry p_entry = malloc(sizeof(struct KEY ## _set_entry)); \
  p_entry->key = key; \
  p_set->value_type = p_entry; \
  p_set->left = NULL; \
  p_set->right = NULL; \
  p_set->size = size; \
  p_set->color = color; \
  return p_set; \
} \
void free_ ## KEY ## _set(KEY ## _set x) { \
  free(x); \
} \
int size_ ## KEY ## _set(KEY ## _set x) { \
  if (!x) return 0; \
  return x->size; \
} \
KEY ## _set_entry find_in_ ## KEY ## _set(KEY ## _set root, KEY key) { \
 if (!root) return NULL; \
 if (IS_LESS ## _(key, root->value_type->key)) return find_in_ ## KEY ## _set(root->left, key); \
 else if (IS_MORE ## _(key, root->value_type->key)) return find_in_ ## KEY ## _set(root->right, key); \
 else return root->value_type; \
} \
/* min is identical to min for vanilla BST */ \
KEY ## _set_entry min_ ## KEY ## _set(KEY ## _set root) { \
  /* return minimum key entry */ \
  if (!root) return NULL; \
  if (!(root->left)) return root->value_type; \
  return min_ ## KEY ## _set(root->left); \
} \
KEY ## _set min_node_ ## KEY ## _set(KEY ## _set root) { \
  /* return minimum rbt node, useful in deletion */ \
  if (!root) return NULL; \
  if (!(root->left)) return root; \
  return min_node_ ## KEY ## _set(root->left); \
} \
KEY ## _set_entry max_ ## KEY ## _set(KEY ## _set root) { \
  /* return maximum key-value pair */ \
  if (!root) return NULL; \
  if (!(root->right)) return root->value_type; \
  return max_ ## KEY ## _set(root->right); \
} \
int isRed_ ## KEY ## _set(KEY ## _set x) { \
  if (!x) return 0; \
  return x->color == RED_COLOR; \
} \
KEY ## _set rotate_left_ ## KEY ## _set(KEY ## _set h) { \
  KEY ## _set x = h->right; \
  h->right = x->left; \
  x->left = h; \
  x->color = h->color; \
  h->color = RED_COLOR; \
  x->size = h->size; \
  h->size = 1 + size_ ## KEY ## _set(h->left) + size_ ## KEY ## _set(h->right); \
  return x; \
  } \
KEY ## _set rotate_right_ ## KEY ## _set(KEY ## _set h) { \
  KEY ## _set x = h->left; \
  h->left = x->right; \
  x->right = h; \
  x->color = h->color; \
  h->color = RED_COLOR; \
  x->size = h->size; \
  h->size = 1 + size_ ## KEY ## _set(h->left) + size_ ## KEY ## _set(h->right); \
  return x; \
} \
void flip_colors_ ## KEY ## _set(KEY ## _set h) { \
  h->color = RED_COLOR; \
  h->left->color = BLACK_COLOR; \
  h->right->color = BLACK_COLOR; \
} \
KEY ## _set insert_helper_ ## KEY ## _set(KEY ## _set root, KEY key) { \
  if (!root) return make_ ## KEY ## _set(key, 1, RED_COLOR); \
  if (IS_LESS ## _(key, root->value_type->key)) root->left = insert_helper_ ## KEY ## _set(root->left, key); \
  else if (IS_MORE ## _(key, root->value_type->key)) root->right = insert_helper_ ## KEY ## _set(root->right, key); \
  if (isRed_ ## KEY ## _set(root->right) && !isRed_ ## KEY ## _set(root->left)) root = rotate_left_ ## KEY ## _set(root); \
  if (isRed_ ## KEY ## _set(root->left) && isRed_ ## KEY ## _set(root->left->left)) root = rotate_right_ ## KEY ## _set(root); \
  if (isRed_ ## KEY ## _set(root->left) && isRed_ ## KEY ## _set(root->right)) flip_colors_ ## KEY ## _set(root); \
  root->size = 1 + size_ ## KEY ## _set(root->left) + size_ ## KEY ## _set(root->right); \
  return root; \
} \
KEY ## _set insert_ ## KEY ## _set(KEY ## _set root, KEY key) { \
  root = insert_helper_ ## KEY ## _set(root, key); \
  root->color = BLACK_COLOR; \
  return root; \
} \
/* deletion procedures in RBTs */ \
/* sedgewick used 'h' as the node parameter */ \
void flip_colors_gen_ ## KEY ## _set(KEY ## _set h) { \
  h->color = !h->color; \
  h->left->color = !h->left->color; \
  h->right->color = !h->right->color; \
} \
KEY ## _set move_red_left_ ## KEY ## _set(KEY ## _set h) { \
  flip_colors_gen_ ## KEY ## _set(h); \
  if (isRed_ ## KEY ## _set(h->right->left)) { \
    h->right = rotate_right_ ## KEY ## _set(h->right); \
    h = rotate_left_ ## KEY ## _set(h); \
    flip_colors_gen_ ## KEY ## _set(h); \
  } \
  return h; \
} \
KEY ## _set move_red_right_ ## KEY ## _set(KEY ## _set h) { \
  flip_colors_gen_ ## KEY ## _set(h); \
  if (isRed_ ## KEY ## _set(h->left->left)) { \
    h = rotate_right_ ## KEY ## _set(h);\
    flip_colors_gen_ ## KEY ## _set(h); \
  }\
  return h; \
  } \
KEY ## _set balance_ ## KEY ## _set(KEY ## _set h) { \
  if (isRed_ ## KEY ## _set(h->right)) h = rotate_left_ ## KEY ## _set(h); \
  if (isRed_ ## KEY ## _set(h->left) && isRed_ ## KEY ## _set(h->left->left)) h = rotate_right_ ## KEY ## _set(h); \
  if (isRed_ ## KEY ## _set(h->left) && isRed_ ## KEY ## _set(h->right)) flip_colors_gen_ ## KEY ## _set(h);  \
  h->size = 1 + size_ ## KEY ## _set(h->left) + size_ ## KEY ## _set(h->right); \
  return h; \
 } \
KEY ## _set delete_min_helper_ ## KEY ## _set(KEY ## _set h, void (* destruct) (KEY ## _set_entry)) { \
  if (!h->left) { \
    if (h->value_type) { \
      destruct(h->value_type);\
    } \
    \
    free(h); \
    return NULL; \
  } \
  if (!isRed_ ## KEY ## _set(h->left) && !isRed_ ## KEY ## _set(h->left->left)) h = move_red_left_ ## KEY ## _set(h); \
  h->left = delete_min_helper_ ## KEY ## _set(h->left, destruct); \
  return balance_ ## KEY ## _set(h); \
} \
KEY ## _set delete_min_ ## KEY ## _set(KEY ## _set root, void (* destruct) (KEY ## _set_entry)) { \
  if (!root) return NULL;  /* for the case where the tree is empty */ \
  /* below line is in the book (algs 4) but absent from sedgewick paper https://www.cs.princeton.edu/~rs/talks/LLRB/LLRB.pdf */ \
  if (!isRed_ ## KEY ## _set(root->left) && !isRed_ ## KEY ## _set(root->right)) root->color = RED_COLOR; \
  root = delete_min_helper_ ## KEY ## _set(root, destruct); \
  if (root) root->color = BLACK_COLOR; \
  return root; \
} \
KEY ## _set delete_max_helper_ ## KEY ## _set(KEY ## _set h, void (* destruct) (KEY ## _set_entry)) { \
  if (isRed_ ## KEY ## _set(h->left))  h = rotate_right_ ## KEY ## _set(h);\
  if (h->right == NULL) { \
    destruct(h->value_type); \
    free(h); \
    return NULL; \
  } \
  if (!isRed_ ## KEY ## _set(h->right) && !isRed_ ## KEY ## _set(h->right->left)) h = move_red_right_ ## KEY ## _set(h); \
  h->right = delete_max_helper_ ## KEY ## _set(h->right, destruct); \
  return balance_ ## KEY ## _set(h); \
} \
KEY ## _set delete_max_ ## KEY ## _set(KEY ## _set root, void (* destruct) (KEY ## _set_entry)) { \
  if (!isRed_ ## KEY ## _set(root->left) && !isRed_ ## KEY ## _set(root->right)) root->color = RED_COLOR; \
  root = delete_max_helper_ ## KEY ## _set(root, destruct); \
  if (root) root->color = BLACK_COLOR; \
  return root; \
} \
KEY ## _set delete_helper_ ## KEY ## _set(KEY ## _set h, KEY key, void (* destruct) (KEY ## _set_entry)) { \
  if (IS_LESS ## _(key, h->value_type->key)) { \
    if (!isRed_ ## KEY ## _set(h->left) && !isRed_ ## KEY ## _set(h->left->left)) h = move_red_left_ ## KEY ## _set(h); \
    h->left = delete_helper_ ## KEY ## _set(h->left, key, destruct); \
  } \
  else {\
    if (isRed_ ## KEY ## _set(h->left)) h = rotate_right_ ## KEY ## _set(h); \
    int is_l = IS_LESS ## _(key, h->value_type->key); \
    int is_m = IS_MORE ## _(key, h->value_type->key); \
    if (!is_l && !is_m && !h->right) { \
      destruct(h->value_type);\
      free(h); \
      return NULL; \
      } \
    if (!isRed_ ## KEY ## _set(h->right) && !isRed_ ## KEY ## _set(h->right->left)) h = move_red_right_ ## KEY ## _set(h); \
    \
    is_l = IS_LESS ## _(key, h->value_type->key); \
    is_m = IS_MORE ## _(key, h->value_type->key); \
    if (!is_l && !is_m) { \
      KEY ## _set x = min_node_ ## KEY ## _set(h->right); \
      h->value_type->key = x->value_type->key; \
      h->right = delete_min_helper_ ## KEY ## _set(h->right, destruct); \
    } else h->right = delete_helper_ ## KEY ## _set(h->right, key, destruct); \
  } \
  return balance_ ## KEY ## _set(h); \
} \
KEY ## _set delete_ ## KEY ## _set(KEY ## _set root, KEY key, void (* destruct) (KEY ## _set_entry)) { \
  if (!isRed_ ## KEY ## _set(root->left) && !isRed_ ## KEY ## _set(root->right)) root->color = RED_COLOR; \
  root = delete_helper_ ## KEY ## _set(root, key, destruct); \
  if (root) root->color = BLACK_COLOR; \
  return root; \
} \
const List_ ## KEY ## _set_entry recursive_inorder_ ## KEY ## _set(KEY ## _set root) { \
  if (!root) return NULL; \
  return append_List_ ## KEY ## _set_entry(3, recursive_inorder_ ## KEY ## _set(root->left), \
    make_list_ ## KEY ## _set(root->value_type->key), \
    recursive_inorder_ ## KEY ## _set(root->right));\
} \
void helper_free_whole_ ## KEY ## _set(KEY ## _set root, void (* destruct) (KEY ## _set_entry)) { \
  /* one way to do this */ \
  \
  /* while(size_ ## KEY ## _set(root)) { \
    \
    root = delete_min_ ## KEY ## _set(root, destruct); \
  } */ \
  \
  /* only very slightly faster */ \
  if (!root) return;  \
  \
  /* free  */ \
  helper_free_whole_ ## KEY ## _set(root->left, destruct);\
  helper_free_whole_ ## KEY ## _set(root->right, destruct);\
  if (root->value_type) { \
    destruct(root->value_type);\
  } \
  free(root);  \
} \
void free_whole_ ## KEY ## _set(KEY ## _set *root, void (* destruct) (KEY ## _set_entry)) { \
  helper_free_whole_ ## KEY ## _set(*root, destruct); \
  *root = NULL; \
}
#endif
