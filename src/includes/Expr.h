#ifndef EXPR_H
#define EXPR_H
#include "TokenType.h"
#include "Scanner.h"
typedef struct Expr *expr;
typedef struct Env *env;

struct Expr { // this structure is fit for holding a single-token expression as well as much more complicated
  // actually we have combined the lexing and parsing into create_expr
  void *value;
  TokenType expr_type;
  expr next;
};

expr eval(Scanner s, env e);
void print_eval_value(expr exp);
expr create_expr(Token_array t_arr, int *start_index); //creates a linked list on the lines of a LISP list to enable quick processing of the user expression string
#endif
