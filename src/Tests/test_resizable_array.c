#include <stdio.h>
#include "../../includes/resizable_array.h"

ARRAY(int)
typedef char *str;
ARRAY(str)
int main(int argc, char const *argv[]) {
  int_array a;
  init_int_array(&a);

  for (int i = 0; i < 150; i++) {
    add_int_array(a, i);
  }
  printf("a->size = %d\n", a->size);
  printf("a->len = %d\n", a->len);
  for (int i = 0; i < a->size; i++) {
    printf("a[%d] = %d\n", i, a->arr[i]);
  }

  str_array as;
  init_str_array(&as);
  add_str_array(as, "hello");
  add_str_array(as, "jllo");
  add_str_array(as, "kello");
  for (int i = 0; i < as->size; i++) {
    printf("%s\n", as->arr[i]);
  }
  return 0;
}
