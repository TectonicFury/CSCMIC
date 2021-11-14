#define MAX_MEMORY_LEN 1024
#include <stdio.h>

void repl() {
  // initialize stack

}
int *cons(void *x, void *y, int *free_pointer, int *d_cars, int *d_cdrs) {

}
int main(int argc, char const *argv[]) {
  int *the_cars_reg = (int *)malloc(sizeof(int *) * MAX_MEMORY_LEN); //1024 * 8 = 8 kb
  int *the_cdrs_reg = (int *)malloc(sizeof(int *) * MAX_MEMORY_LEN);
  int *root_reg = 0;
  int *stack_reg;
  // for use in ec-eval
  int *env_reg;
  int *continue_reg;
  int *val_reg;
  int *exp_reg;
  int *env_reg;
  int *unev_reg;
  int *proc_reg;


  int free_index_reg = 0;

  return 0;
}
