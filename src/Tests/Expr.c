#include <stdio.h>
#include <stdlib.h>
#include "../includes/Expr.h"

// code and data are same

HashTable(str, expr, str_equal, str_hash)

Scanner s;

struct Env {
  str_expr_hash_table table;
  env next;
};

int is_pair(expr exp);
int is_variable(expr exp);

void make_env(env *environment) {
  env e = malloc(sizeof(struct Env));
  init_str_expr_hash_table(&(e->table));
  e->next = NULL;
  *environment = e;
}

expr eval_expr(expr exp, env e);

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
  if (!exp) return;
  if (is_pair(exp)) {
    printf("(");
    print_expr(exp->value);
    printf(")");
    print_expr(exp->next);
    return;
  }
  if (is_number(exp)) {
    if (exp->next == NULL) {
      printf("%f", *((double*)exp->value));
      return;
    }
      printf("%f ", *((double*)exp->value));
      print_expr(exp->next);
      return;
    }
  if (is_string(exp)) {
    if (exp->next == NULL) {
      printf("%s", (char*)exp->value);
      return;
    }
    printf("%s ", (char*)exp->value);
    print_expr(exp->next);
    return;
  }
  if (is_variable(exp) || is_keyword(exp)) {
    if (exp->next == NULL) {
      // printf("exp = %p\n", exp);
      printf("%s", (char*)exp->value);
      return;
    }
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
  } if (exp->expr_type == LESS) {
    printf("< ");
    print_expr(exp->next);
    return;
  }
}

int is_pair(expr exp) {
  if (exp->expr_type == LEFT_PAREN) return 1;
  return 0;
}

int is_self_evaluating(expr exp) {
  printf("in is_self_evaluating\n");
  return is_number(exp) || is_string(exp) || exp->expr_type == NIL || exp->expr_type == FALSE;
}

int is_variable(expr exp) {
  switch (exp->expr_type) {
    case IDENTIFIER:
    case PLUS:
    case MINUS:
    case STAR:
    case SLASH:
    case LESS:
    case LESS_EQUAL:
    case GREATER:
    case GREATER_EQUAL:
    case EQUAL:
    case ABS:
      return 1;
    default:
      return 0;
  }
  // return exp->expr_type == IDENTIFIER || exp->expr_type == PLUS || exp->expr_type == MINUS || exp->expr_type == STAR || exp->expr_type == SLASH || exp->expr_type == LESS;
}

int is_definition(expr exp) {
  if (is_pair(exp) && ((expr)exp->value)->expr_type == DEFINE) return 1;
  return 0;
}

int is_lambda(expr exp) {
  if (is_pair(exp) && ((expr)exp->value)->expr_type == LAMBDA) return 1;
  return 0;
}

expr eval_self_evaluating(expr exp, env e) {
  return exp;
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

expr lambda_parameters(expr exp) {
  // this works الحمد لله
  return ((expr)exp->value)->next->value;
}

expr lambda_body(expr exp) {
  // this works الحمد لله
  return ((expr)exp->value)->next->next;
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

expr eval_definition(expr exp, env e) {
  insert_str_expr_hash_table(&e->table, definition_variable(exp), eval_expr(definition_value(exp), e), destroy_str_expr_pair);
  expr ret_val = malloc(sizeof(struct Expr));
  ret_val->expr_type = STRING;
  ret_val->value = "ok";
  return ret_val;
}

expr make_procedure(expr exp, env e) {
  expr s = malloc(sizeof(struct Expr));
  s->expr_type = LEFT_PAREN;

  expr t = malloc(sizeof(struct Expr));
  t->expr_type = PROCEDURE;
  t->value = lambda_parameters(exp); // this link contains the list pf params of the procedure
  t->next = lambda_body(exp); // this link contains the body of the procedure

  s->value = t;
  s->next = (expr)e; //just storing a link, will need to recast to env
  return s;
}

int is_if(expr exp) {
  return is_pair(exp) && ((expr)exp->value)->expr_type == IF;
}

expr if_predicate(expr exp) {
  return ((expr)exp->value)->next;
}
expr if_consequent(expr exp) {
  return((expr) exp->value)->next->next;
}
expr if_alternative(expr exp) {
  return ((expr)exp->value)->next->next->next;
}

int is_true(expr exp) {
  return exp->expr_type != NIL && exp->expr_type != FALSE;
}
int is_application(expr exp) {
  return is_pair(exp);
}
expr eval_if(expr exp, env e) {
  if (is_true(eval_expr(if_predicate(exp), e))) return eval_expr(if_consequent(exp), e);
  return eval_expr(if_alternative(exp), e);
}

expr eval_lambda(expr exp, env e) {
  return make_procedure(exp, e);
}

expr proc_body(expr exp) {
  return ((expr)exp->value)->next;
}

expr proc_parameters(expr exp) {
  return (expr)((expr)exp->value)->value;
}

env proc_env(expr exp) {
  return (env)exp->next;
}

int is_compound_proc(expr exp) {
  if (is_pair(exp) && ((expr)exp->value)->expr_type == PROCEDURE) {
    return 1;
  }
  return 0;
}

expr eval_sequence(expr exps, env e) {
  if (exps->next == NULL) {
    return eval_expr(exps, e);
  }
  eval_expr(exps, e);
  return eval_sequence(exps->next, e);
}

env extend_environment(expr vars, expr vals, env base_env) {
  env new_e;
  make_env(&new_e);
  while (vars) {
    insert_str_expr_hash_table(&new_e->table, (str)vars->value, vals, destroy_str_expr_pair);
    vars = vars->next;
    vals = vals->next;
  }
  new_e->next = base_env;
  return new_e;
}

expr apply(expr proc, expr arguments, env e) {
  if (is_compound_proc(proc)) {
    return eval_sequence(proc_body(proc), extend_environment(proc_parameters(proc), arguments, proc_env(proc)));
  }

  // else it is a primitive function
  while (e->next != NULL) { // primitive frocedure
    e = e->next;
  }
  str_expr_pairST p = find_in_str_expr_hash_table(e->table, (str)proc->value);
  if (p) { // this means that we have found a primitive function
    if (strcmp((str)(p->value->value), "+") == 0) {
      double sum = 0;

      while (arguments) {
        double i = *((double*)arguments->value);
        sum += i;
        arguments = arguments->next;
      }
      double *d = malloc(sizeof(double));
      *d = sum;
      expr res = malloc(sizeof(struct Expr));
      res->expr_type = NUMBER;
      res->value = d;
      res->next = NULL;
      return res;
    }
    else if (strcmp((str)(p->value->value), "-") == 0) {
      double sum = 0;
      double i = *((double*)arguments->value);
      sum = i;
      arguments = arguments->next;
      while (arguments) {
        i = *((double*)arguments->value);
        sum -= i;
        arguments = arguments->next;
      }
      double *d = malloc(sizeof(double));
      *d = sum;
      expr res = malloc(sizeof(struct Expr));
      res->expr_type = NUMBER;
      res->value = d;
      res->next = NULL;
      return res;
    } else if (strcmp((str)(p->value->value), "*") == 0) {
      double prod = 1;
      double i = *(double*)arguments->value;
      prod = i;
      arguments = arguments->next;
      while (arguments) {
        i = *(double*)arguments->value;
        prod *= i;
        arguments = arguments->next;
      }
      double *d = malloc(sizeof(double));
      *d = prod;
      expr res = malloc(sizeof(struct Expr));
      res->expr_type = NUMBER;
      res->value = d;
      res->next = NULL;
      return res;
    }
    else if (strcmp((str)(p->value->value), "<") == 0) {
      if (!arguments) {
        expr res = malloc(sizeof(struct Expr));
        res->expr_type = TRUE;
        res->value = "true";
        res->next = NULL;
        return res;
      }
      while (arguments) {
        if (!arguments->next) {
          expr res = malloc(sizeof(struct Expr));
          res->expr_type = TRUE;
          res->value = "true";
          res->next = NULL;
          return res;
        }
        if ((*(double*)arguments->value >= *(double*)arguments->next->value)) {
          expr res = malloc(sizeof(struct Expr));
          res->expr_type = FALSE;
          res->value = "false";
          res->next = NULL;
          return res;
        }
        arguments = arguments->next;
      }
    }
  }
  return NULL;
}

expr copy_expr(expr exp) { // needed for lookups
  if (!exp) return NULL;
  expr e = malloc(sizeof(struct Expr));
  if (exp->expr_type == LEFT_PAREN) {
    e->expr_type = LEFT_PAREN;
    e->value = copy_expr((expr)exp->value);
    e->next = NULL;
    return e;
  } else if (exp->expr_type == NUMBER) {
    e->expr_type = NUMBER;
    e->value = exp->value;
    e->next = copy_expr(exp->next);
    return e;
  } else if (exp->expr_type == PLUS) {
    e->expr_type = PLUS;
    e->value = "+";
    e->next = NULL;
    return e;
  } else if (exp->expr_type == MINUS) {
    e->expr_type = MINUS;
    e->value = "-";
    e->next =  NULL;
    return e;
  } else if (exp->expr_type == STAR) {
    e->expr_type = STAR;
    e->value = "*";
    e->next = NULL;
    return e;
  } else if (exp->expr_type == SLASH) {
    e->expr_type = SLASH;
    e->value = "/";
    e->next = NULL;
    return e;
  } else if (exp->expr_type == LESS) {
    e->expr_type = LESS;
    e->value = "<";
    e->next = NULL;
    return e;
  }
  // else if PROCEDURE
  return NULL;
}

expr lookup_variable_value(expr exp, env e) {
  while (e != NULL) {
    str_expr_pairST p_val = find_in_str_expr_hash_table(e->table, exp->value);
    if (p_val) {
      if (is_pair(p_val->value) && ((expr)p_val->value->value)->expr_type == PROCEDURE) {
        return p_val->value;
      }
      // return (p_val->value);
      return copy_expr(p_val->value);
    }
    e = e->next;
  }
  printf("variable %s not found\n", (char*)exp->value);
  return NULL;
}

expr operator(expr exp) {
  // print_expr(car(exp));
  return car(exp);
}

expr operands(expr exp) {
  return car(exp)->next;
}

expr list_of_values(expr exps, env e) {
  expr u = exps;
  expr t = eval_expr(u, e);
  expr v = t;
  u = u->next;
  while (u) {
    t->next = eval_expr(u, e);
    t = t->next;
    u = u->next;
  }
  t->next = NULL;
  return v;
}

expr eval_expr(expr exp, env e) {
  if (is_self_evaluating(exp)) {
    return eval_self_evaluating(exp, e);
  } else if (is_variable(exp)) {
    return lookup_variable_value(exp, e);
  } else if (is_definition(exp)) {
    return eval_definition(exp, e);
  } else if (is_if(exp)) {
    return eval_if(exp, e);
  } else if (is_lambda(exp)) {
    return eval_lambda(exp, e);
  } else if (is_application(exp)) {
    expr res = apply(eval_expr(operator(exp), e), list_of_values(operands(exp), e), e);
    return res;
  }
  printf("returning NULL\n");
  printf("returning NULL\n");
  return NULL;
}

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

expr eval(Scanner s, env e) {
  int start_index = 0;
  expr exp = create_expr(s->tokens, &start_index);
  expr val = eval_expr(exp, e);
  // free_expr(e); //need to free the space before returning
  return val;
}

void print_eval_value(expr exp) {
  if (exp->expr_type == NUMBER) {
    printf("%f\n", *((double*)exp->value));
  } else if (exp->expr_type == STRING) {
    printf("%s\n", (char*)exp->value);
  } else if (exp->expr_type == LEFT_PAREN) {
    print_eval_value((expr)exp->value);
  } else if (exp->expr_type == PROCEDURE) {
    printf("[compound-procedure : parameters = ");
    print_expr((expr)exp->value);
    printf(", body = ");
    print_expr(exp->next);
    printf("]\n");
  } else print_expr(exp);
}

int main(int argc, char const *argv[]) {
  // Scanner s;
  char input[MAX_IN_LEN];
  env environment;
  make_env(&environment);
  insert_str_expr_hash_table(&environment->table, "+", make_expr_node(make_token(PLUS, "+", NULL, 0)), destroy_str_expr_pair);
  insert_str_expr_hash_table(&environment->table, "-", make_expr_node(make_token(MINUS, "-", NULL, 0)), destroy_str_expr_pair);
  insert_str_expr_hash_table(&environment->table, "/", make_expr_node(make_token(SLASH, "/", NULL, 0)), destroy_str_expr_pair);
  insert_str_expr_hash_table(&environment->table, "*", make_expr_node(make_token(STAR, "*", NULL, 0)), destroy_str_expr_pair);
  insert_str_expr_hash_table(&environment->table, "=", make_expr_node(make_token(EQUAL, "=", NULL, 0)), destroy_str_expr_pair);
  insert_str_expr_hash_table(&environment->table, "<", make_expr_node(make_token(LESS, "<", NULL, 0)), destroy_str_expr_pair);
  insert_str_expr_hash_table(&environment->table, "abs", make_expr_node(make_token(ABS, "abs", NULL, 0)), destroy_str_expr_pair);
  // just add any function you want to be really fast as a primitive of the language
  // eg. for adding fibonacci : insert_str_expr_hash_table(&environment->table, "fib", make_expr_node...)

  // printf("environment->table = %p\n", environment->table);
  while (1) {
    printf("\n]=> ");
    init_scanner(&s, input);
    get_expression(input);
    strcpy(s->source, input);
    scan_tokens(s);

    printf("\n;value:\n");
    print_eval_value(eval(s, environment));
    // take scanner input and make it into a tree-list structure

    // free_scanner(&s); // prevent memory leaks
  }
  return 0;
}
