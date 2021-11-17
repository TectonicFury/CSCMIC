#include <stdio.h>
#include "../includes/Scanner.h"

int main(int argc, char const *argv[]) {
  Scanner s;
  char input[MAX_IN_LEN];

  while (1) {
    printf("\n]=> ");
    init_scanner(&s, input);
    get_expression(input);
    printf("input = %s\n", input);
    strcpy(s->source, input);
    scan_tokens(s);
    // printf("tokens->size = %d\n", s->tokens->size);
    // for (int i = 0; i < s->tokens->size; i++) {
    //   printf("%s\n", s->tokens->arr[i]->lexeme);
    // }

    // take scanner input and make it into a tree-list structure
    
    free_scanner(&s); // prevent memory leaks
  }




  return 0;
}
