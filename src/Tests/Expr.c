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
int is_pair(expr exp);
int is_variable(expr exp);

void make_env(env *environment) {
  env e = malloc(sizeof(struct Env));
  init_str_expr_hash_table(&(e->table));
  printf("env h_table %p\n", e->table);
  printf("env ht pointer = %p\n", e->table->ht);
  e->next = NULL;
  *environment = e;
}
Eval_Value make_value(expr exp) {
  Eval_Value v = malloc(sizeof(struct Eval_Value));
  switch (exp->expr_type) {
    case NUMBER:
      v->type = NUMBER_VAL;
      v->value = exp->value;
      return v;
    case STRING:
      v->type = STRING_VAL;
      v->value = exp->value;
      return v;
    case LEFT_PAREN:
      if (((expr)exp->value)->expr_type == DEFINE) {
        v->type = STRING_VAL;
        v->value = " ok\n";
        return v;
      }
      v->type = LIST_VAL;
      v->value = exp;
      return v;
      // printf("returning NULL\n");
      // return NULL;
    default:
      printf("in progress\n");
      return NULL;
  }
}

expr make_expr_node(Token t) {
  expr e = malloc(sizeof(struct Expr));
  e->expr_type = t->type;
  if (t->type == STRING || t->type == NUMBER) e->value = t->literal;
  else if (t->type == LEFT_PAREN) e->value = NULL; // for dummy value for the other tokens like LEFT_PAREN, this will point to the nested expr
  else e->value = t->lexeme; // this will be used in variable lookup and for printing keywords
  e->next = NULL;
  return e;
}

int is_number(expr exp) {
  return exp->expr_type == NUMBER;
}

int is_string(expr exp) {
  return exp->expr_type == STRING;
}
int is_keyword(expr exp) {
  switch (exp->expr_type) {
    case IF:
    case TRUE:
    case FALSE:
    case NIL:
    case DEFINE:
    case BEGIN:
    case LET:
    case LAMBDA:
    case COND:
    case AND:
    case NOT:
    case OR:
    case ELSE:
    case SET:
      return 1;
    default:
      return 0;
  }
}
void print_expr(expr exp) {
  // printf("\nprint_expr >>\n");
  if (!exp) return;
  if (is_pair(exp)) {
    printf(" (");
    print_expr(exp->value);
    printf(") ");
    print_expr(exp->next);
    return;
  }
  if (is_number(exp)) {
      printf("%f ", *((double*)exp->value));
      print_expr(exp->next);
      return;
    }
  if (is_string(exp)) {
    printf("%s ", (char*)exp->value);
    print_expr(exp->next);
    return;
  }
  if (is_variable(exp) || is_keyword(exp)) {
    // printf("%p\n", exp->value);
    printf("%s ", (char*)exp->value);
    print_expr(exp->next);
    return;
  }
  if (exp->expr_type == SLASH) {
    printf("/ ");
    print_expr(exp->next);
    return;
  }
  if (exp->expr_type == STAR) {
    printf("* ");
    print_expr(exp->next);
    return;
  }
  if (exp->expr_type == PLUS) {
    printf("+ ");
    print_expr(exp->next);
    return;
  }
  if (exp->expr_type == MINUS) {
    printf("- ");
    print_expr(exp->next);
    return;
  }
}
int is_pair(expr exp) {
  // printf("exp pointer lambda = %p\n", exp);
  if (exp->expr_type == LEFT_PAREN) return 1;
  return 0;
}

int is_self_evaluating(expr exp) {
  return is_number(exp) || is_string(exp);
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
expr car(expr exp) {
  if (is_pair(exp)) {
    return exp->value;
  }
  printf("Car cannot be used for exp\n");
  return NULL;
}

expr cadr(expr exp) {
  if (is_pair(exp)) {
    return ((expr)exp->value)->next;
  }
  return NULL;
}
expr caddr(expr exp) {
  if (is_pair(exp)) {
    return ((expr)exp->value)->next->next;
  }
  return NULL;
}
str definition_variable(expr exp) {
  expr t = cadr(exp);
  if (is_variable(t)) { // is cadr a symbol
    return t->value;
  } else if (is_pair(t)) {
    return (char*)car(t)->value;
  }
  return NULL;
}
expr make_lambda(expr args, expr body) {
  expr arg_list = malloc(sizeof(struct Expr));
  arg_list->expr_type = LEFT_PAREN;
  arg_list->value = args;
  arg_list->next = body;

  expr lambda_node = malloc(sizeof(struct Expr));
  lambda_node->expr_type = LAMBDA;
  lambda_node->value = "lambda";
  lambda_node->next = arg_list;

  expr lambda_paren = malloc(sizeof(struct Expr));
  lambda_paren->expr_type = LEFT_PAREN;
  lambda_paren->value = lambda_node;
  lambda_paren->next = NULL; // missing it causes high valued reference memory error
  return lambda_paren;
}

expr definition_value(expr exp) {
  expr t = cadr(exp);
  if (is_variable(t)) {
    return t->next;
  } else {
    return make_lambda(((expr)t->value)->next, t->next);
  }
  return NULL;
}
void destroy_str_expr_pair(str_expr_pairST p) {

}
Eval_Value eval_definition(expr exp, env e) {
  printf("definition_variable = %s\n", definition_variable(exp));
  insert_str_expr_hash_table(&e->table, definition_variable(exp), definition_value(exp), destroy_str_expr_pair);
  return make_value(exp);
}
Eval_Value lookup_variable_value(expr exp, env e) {
  while (e != NULL) {
    str_expr_pairST p_val = find_in_str_expr_hash_table(e->table, exp->value);
    if (p_val) {
      printf("found lookup\n");
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
  printf("Printing >>\n");
  print_expr(exp);
  Eval_Value val = eval_expr(exp, e);
  // free_expr(e); //need to free the space before returning
  return val;
}

void print_eval_value(Eval_Value v) {
  if (v->type == NUMBER_VAL) {
    printf("%f\n", *((double*)v->value));
  } else if (v->type == STRING_VAL) {
    printf("%s\n", (char*)v->value);
  } else if (v->type == LIST_VAL) {
    print_expr((expr)v->value);
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
