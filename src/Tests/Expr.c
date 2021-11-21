#include <stdio.h>
#include <stdlib.h>
#include "../includes/Expr.h"
// #include "../includes/Scanner.h"

// code and data are same

HashTable(str, expr, str_equal, str_hash)

struct Env {
  str_expr_hash_table table;
  env next;
};

void make_env(env *environment) {
  env e = malloc(sizeof(struct Env));
  init_str_expr_hash_table(&(e->table));
  printf("env h_table %p\n", e->table);
  printf("env ht pointer = %p\n", e->table->ht);
  e->next = NULL;
  *environment = e;
}
Eval_Value make_value(expr e) {
  Eval_Value v = malloc(sizeof(struct Eval_Value));
  switch (e->expr_type) {
    case NUMBER:
      v->type = NUMBER_VAL;
      v->value = e->value;
      return v;
    case STRING:
      v->type = STRING_VAL;
      v->value = e->value;
      return v;
    case LEFT_PAREN:
      if (((expr)e->value)->expr_type == DEFINE) {
        v->type = STRING_VAL;
        v->value = "; ok\n";
        return v;
      }
      return NULL;
    default:
      printf("in progress\n");
      return NULL;
  }
}

expr make_expr_node(Token t) {
  expr e = malloc(sizeof(struct Expr));
  e->expr_type = t->type;
  if (t->type == STRING || t->type == NUMBER) e->value = t->literal;
  else if (t->type == IDENTIFIER) e->value = t->lexeme; // this will be used in variable lookup
  else e->value = NULL; // for dummy value for the other tokens like LEFT_PAREN, this will point to the nested expr
  // e->next = NULL;
  return e;
}

int is_self_evaluating(expr exp) {
  return exp->expr_type == NUMBER || exp->expr_type == STRING;
}

int is_variable(expr exp) {
  return exp->expr_type == IDENTIFIER;
}
int is_definition(expr exp) {
  if (((expr)exp->value)->expr_type == DEFINE) return 1;
  return 0;
}

Eval_Value eval_self_evaluating(expr exp, env e) {
  return make_value(exp);
}
str definition_variable(expr exp) {
  expr t = exp;
  t = t->value;
  while (t->expr_type != IDENTIFIER) {
    t = t->next;
  }
  return t->value;
}
expr definiton_value(expr exp) {
  expr t = exp;
  t = t->value;
  while (t->expr_type != IDENTIFIER) {
    t = t->next;
  }
  return t->next;
}
void destroy_str_expr_pair(str_expr_pairST p) {

}
Eval_Value eval_definition(expr exp, env e) {
  insert_str_expr_hash_table(&e->table, definition_variable(exp), definiton_value(exp), destroy_str_expr_pair);
  return make_value(exp);
}
Eval_Value lookup_variable_value(expr exp, env e) {
  while (e != NULL) {
    str_expr_pairST p_val = find_in_str_expr_hash_table(e->table, exp->value);
    if (p_val) {
      return make_value(p_val->value);
    }
    e = e->next;
  }
  printf("variable %s not found\n", (char*)exp->value);
  return NULL;
}
Eval_Value eval_expr(expr exp, env e) {
  if (is_self_evaluating(exp)) {
    return eval_self_evaluating(exp, e);
  } else if (is_variable(exp)) {
    return lookup_variable_value(exp, e);
  } else if (is_definition(exp)) {
    // printf("DEFINE found\n");
    return eval_definition(exp, e);
  }
  return NULL;
}


// int is_quoted(expr e);
// int is_assignment(expr e);
// int is_definition(expr e);
// int is_if(expr e);
// int is_lambda(expr e);
// int is_begin(expr e);
// int is_cond(expr e);
// int is_application(expr r);
//
// int is_pair(expr e) {
//   if (e->type_tag == PAIR) return 1;
//   return 0;
// }
//
// int is_symbol(expr e) {
//   return e->type_tag == SYMBOL;
// }
// int is_number(expr e) {
//   return (e->type_tag == NUMBER);
// }
//
// int is_string(expr e) {
//   return (e->type_tag == STRING);
// }
//
// int is_tagged_list(expr e, EXPR_TYPE type_tag) {
//   if (is_pair(e) && ((expr)e->value)->type_tag == type_tag) return 1;
//   return 0;
// }

expr create_expr(Token_array t_arr, int *start_index) {
  // creates the expr using the user input, essentially a parsing step
  expr e;
  Token t;

  t = t_arr->arr[*start_index]; // token at position *start_index
  // switch (t->type) {
  //   case LEFT_PAREN:
  //     printf("t->type = %s\n", "LEFT_PAREN");
  //     break;
  //   case RIGHT_PAREN:
  //     printf("t->type = %s\n", "RIGHT_PAREN");
  //     break;
  //   case IDENTIFIER:
  //     printf("t->type = %s\n", "IDENTIFIER");
  //     break;
  //   case NUMBER:
  //     printf("t->type = %s\n", "NUMBER");
  //     break;
  //   default:
  //     break;
  // }
  if (t->type == _EOF_) return NULL;

  if (t->type == RIGHT_PAREN) {
    *start_index = *start_index + 1; // consume the right parenthesis and represent it with NULL
    return NULL;
  }

  if (t->type == LEFT_PAREN) {
    e = malloc(sizeof(struct Expr));
    e->expr_type = LEFT_PAREN;
    *start_index = *start_index + 1;
    e->value = create_expr(t_arr, start_index); // make a new level in the tree,
    e->next = create_expr(t_arr, start_index); // get the next atoms or lists
    return e;
  }

  e = make_expr_node(t);
  *start_index = *start_index + 1;
  e->next = create_expr(t_arr, start_index);
  return e;
}

Eval_Value eval(Scanner s, env e) {
  int start_index = 0;
  expr exp = create_expr(s->tokens, &start_index);
  Eval_Value val = eval_expr(exp, e);
  // free_expr(e); //need to free the space before returning
  return val;
}

void print_eval_value(Eval_Value v) {
  if (v->type == NUMBER_VAL) {
    printf("%f\n", *((double*)v->value));
  } else if (v->type == STRING_VAL) {
    printf("%s\n", (char*)v->value);
  }
}

int main(int argc, char const *argv[]) {
  Scanner s;
  char input[MAX_IN_LEN];
  env environment;
  make_env(&environment);
  printf("environment->table = %p\n", environment->table);
  while (1) {
    printf("\n]=> ");
    init_scanner(&s, input);
    get_expression(input);
    // printf("input = %s\n", input);
    strcpy(s->source, input);
    scan_tokens(s);
    // printf("tokens->size = %d\n", s->tokens->size);
    // for (int i = 0; i < s->tokens->size; i++) {
    //   printf("%s\n", s->tokens->arr[i]->lexeme);
    // }
    printf("\n;value: ");
    print_eval_value(eval(s, environment));
    // take scanner input and make it into a tree-list structure

    // free_scanner(&s); // prevent memory leaks
  }
  return 0;
}

// expr copy_expr(expr e) { //recursively copies the whole list
//   if (!e) {
//     return NULL;
//   }
//   expr e_copy = (expr)malloc(sizeof(struct Expr));
//   if (e->type_tag == '(') {
//     e_copy->type_tag = '(';
//     e_copy->value  = copy_expr(e->value);
//     e_copy->next = copy_expr(e->next);
//   } else if (e->type_tag == 'o') {
//     e_copy->type_tag = 'o';
//     char *cp = (char*)malloc(sizeof(char));
//     *cp = *((char*)e->value);
//     e_copy->value = cp;
//     e_copy->next = copy_expr(e->next);
//   } else {
//     e_copy->type_tag = 'n';
//     double *dp = (double*)malloc(sizeof(double));
//     *dp = *((double*)e->value);
//     e_copy->value = dp;
//     e_copy->next = copy_expr(e->next);
//   }
//   return e_copy;
// }
//
// expr copy_single_expr(expr e) { //copies only the first expr, doesn't copy the trailing exprs in the list
//   if (!e) {
//     return NULL;
//   }
//   expr e_single_copy = (expr)malloc(sizeof(struct Expr));
//   if (e->type_tag == '(') {
//     //type_tag is '(' for parenthesised expression
//     e_single_copy->type_tag = '(';
//     e_single_copy->value  = copy_expr(e->value); //if its a list, then we copy the whole underlying list
//   } else if (e->type_tag == 'o') {
//     //type_tag is 'o' for operator
//     e_single_copy->type_tag = 'o';
//     char *cp = (char*)malloc(sizeof(char));
//     *cp = *((char*)e->value);
//     e_single_copy->value = cp;
//   } else {
//     // the type_tag is 'n' for numeric
//     e_single_copy->type_tag = 'n';
//     double *dp = (double*)malloc(sizeof(double));
//     *dp = *((double*)e->value);
//     e_single_copy->value = dp;
//   }
//   return e_single_copy;
// }
//
// void free_expr(expr e) { //recursively frees the memory allocated for expression
//   if (!e) return;
//   free_expr(e->next);
//   e->next = NULL;
//   if (e->type_tag == '(') {
//     free_expr(e->value);
//     e->value = NULL;
//   } else {
//     free(e->value);
//     e->value = NULL;
//     free(e);
//   }
// }
//
// void print_expr(expr e) {//prints the expr
//   expr t = e;
//   while (t != NULL) {
//     if (t->type_tag == '(') {
//       printf("(");
//       print_expr(t->value);
//       printf(") ");
//       t = t->next;
//     } else if (t->type_tag == 'n') {
//       printf(" %.2lf ", *((double*) t->value));
//       t = t->next;
//     } else if (t->type_tag == 'o') {
//       printf("%c ", *((char*)t->value));
//       t = t->next;
//     }
//   }
// }
//
// double number(expr e) {
//   return *((double*)e->value);
// }
//
// int is_sum(expr e) {//checks if the expression is a sum
//   while (e) {
//     if (e->type_tag == 'o' && *((char*)e->value) == '+') {
//       return 1;
//     }
//     e = e->next;
//   }
//   return 0;
// }
//
// int is_product(expr e) {//checks if the expression is a product
//   while (e) {
//     if (e->type_tag == 'o' && *((char*)e->value) == '*') {
//       return 1;
//     }
//     e = e->next;
//   }
//   return 0;
// }
//
// expr addend(expr e) {//get all the terms before the '+' sign
//   expr e_addend  = copy_single_expr(e);
//   expr t = e_addend;
//   e = e->next;
//   while (e && !(e->type_tag == 'o' && *((char*)e->value) == '+')) {
//     t->next = copy_single_expr(e);
//     t = t->next;
//     e = e->next;
//   }
//   t->next = NULL;
//   return e_addend;
// }
//
// expr augend(expr e) {//get all the terms after the '+' sign
//   int flag = 0;
//
//   while (!flag) {
//     if (e->type_tag == 'o' && *((char*)e->value) == '+') {
//       flag = 1;
//       break;
//     }
//     e = e->next;
//   }
//   e = e->next;
//   expr e_augend = copy_single_expr(e);
//   expr t = e_augend;
//   e = e->next;
//   while (e) {
//     t->next = copy_single_expr(e);
//     t = t->next;
//     e = e->next;
//   }
//   t->next = NULL;
//   return e_augend;
// }
//
// expr multiplier(expr e) {//get all the terms before the '*' sign
//   expr e_multiplier  = copy_single_expr(e);
//   expr t = e_multiplier;
//   e = e->next;
//   while (e && !(e->type_tag == 'o' && *((char*)e->value) == '*')) {
//     t->next = copy_single_expr(e);
//     t = t->next;
//     e = e->next;
//   }
//   t->next = NULL;
//   return e_multiplier;
// }
//
// expr multiplicand(expr e) {//get all the terms after the '*' sign
//   int flag = 0;
//
//   while (!flag) {
//     if (e->type_tag == 'o' && *((char*)e->value) == '*') {
//       flag = 1;
//       break;
//     }
//     e = e->next;
//   }
//   e = e->next;
//   expr e_multiplicand = copy_single_expr(e);
//   expr t = e_multiplicand;
//   e = e->next;
//   while (e) {
//     t->next = copy_single_expr(e);
//     t = t->next;
//     e = e->next;
//   }
//   t->next = NULL;
//   return e_multiplicand;
// }
//
// int is_expr_paren(expr e) { //does it have extraneous parenthesis like ((3+4))
//   return (e->type_tag == '(');
// }
// int get_user_expr(char s[], int lim) {//get input string from user
//   int c, i;
//   for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; i++) {
//     s[i] = c;
//     printf("c is %c\n", c);
//   }
// printf("c is %c\n", c);
//   if (c == '\n') {
//     s[i] = '\0';
//   } else if (c == EOF) {
//     exit(0);
//   }
//   return i; //returns the length of the whole string
// }
//
// int is_numeric(char c) {
//   return ((c >= '0' && c <= '9') || c == '.');
// }
// int is_operator(char c) {
//   return (c == '+' || c == '*');
// }
// double get_number(char* expr, int* start, int lim) {
//   while (expr[*start] == ' ') {
//     *start = *start + 1;
//   }
//   double val = 0;
//   int flag = 0;
//   double decimal_part = 0;
//   int frac_digits = 0;
//   double x_digit = 0;
//   while((expr[*start] >= '0' && expr[*start] <= '9') || expr[*start] == '.') {
//     if (expr[*start] == '.') {
//       flag = 1;
//       *start = *start + 1;
//       continue;
//     }
//     if (!flag) {
//       val = val * 10 + (expr[*start] - '0');
//     } else {
//       x_digit = (expr[*start] - '0');
//       for (size_t k = 0; k <= frac_digits; k++) {
//         x_digit = x_digit/10.0;
//       }
//       decimal_part = decimal_part + x_digit;
//       frac_digits++;
//     }
//     *start = *start + 1;
//   }
//   return val + decimal_part;
// }
