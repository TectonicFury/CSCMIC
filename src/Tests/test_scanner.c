#include <stdio.h>
#include "../includes/Scanner.h"

int main(int argc, char const *argv[]) {
  Scanner s;
  char input[MAX_IN_LEN];
  init_scanner(&s, input);
  Token t;
  t = make_token(LESS, "<", NULL, 1);
  printf("t->lexeme = %s\n", t->lexeme);

  while (1) {
    printf("\n]=> ");
    get_expression(input);
    // printf("\ninput is = \n%s\n", input);
    strcpy(s->source, input);
    scan_tokens(s);
    for (int i = 0; i < s->tokens->size; i++) {
      printf("%s\n", s->tokens->arr[i]->lexeme);
    }
  }




  return 0;
}
