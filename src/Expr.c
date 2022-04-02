#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "includes/Expr.h"

// code and data are same

int expr_equal_(expr exp1, expr exp2) { // this will check the variable if it is equal to a key within the hash table
  return (exp1->expr_type == exp2->expr_type) && (strcmp(exp1->first_value, exp2->first_value) == 0);
}
unsigned int expr_hash_(expr exp, int M) {
  // djb2
  // assuming an expr of IDENTIFIER_TYPE or STRING_TYPE
  unsigned int hash = 5381;
  int c;
  char *s = exp->first_value;
  c = *s;
  while (c != '\0') {
    hash = hash * 33 + c;
    s++;
    c = *s;
  }
  return hash % M;
}

HashTable(expr, expr, expr_equal, expr_hash)

void destroy_expr_expr_pair(expr_expr_pairST p);

struct Env {
  expr_expr_hash_table table;
  env next;
};

expr the_empty_list;
expr the_true;
expr the_false;

int main(int argc, char const *argv[]) {
  Scanner s;
  char input[MAX_IN_LEN];
  env environment;
  make_env(&environment);
  // loading primitives into the global environment
  insert_expr_expr_hash_table(&environment->table, make_expr_node(make_token(PLUS, "+", NULL, 0)), make_expr_node(make_token(PLUS, "+", NULL, 0)), destroy_expr_expr_pair);
  insert_expr_expr_hash_table(&environment->table, make_expr_node(make_token(MINUS, "-", NULL, 0)), make_expr_node(make_token(MINUS, "-", NULL, 0)), destroy_expr_expr_pair);
  insert_expr_expr_hash_table(&environment->table, make_expr_node(make_token(SLASH, "/", NULL, 0)), make_expr_node(make_token(SLASH, "/", NULL, 0)), destroy_expr_expr_pair);
  insert_expr_expr_hash_table(&environment->table, make_expr_node(make_token(STAR, "*", NULL, 0)), make_expr_node(make_token(STAR, "*", NULL, 0)), destroy_expr_expr_pair);
  insert_expr_expr_hash_table(&environment->table, make_expr_node(make_token(EQUAL, "=", NULL, 0)), make_expr_node(make_token(EQUAL, "=", NULL, 0)), destroy_expr_expr_pair);
  insert_expr_expr_hash_table(&environment->table, make_expr_node(make_token(LESS, "<", NULL, 0)), make_expr_node(make_token(LESS, "<", NULL, 0)), destroy_expr_expr_pair);
  insert_expr_expr_hash_table(&environment->table, make_expr_node(make_token(IDENTIFIER, "abs", NULL, 0)), make_expr_node(make_token(IDENTIFIER, "abs", NULL, 0)), destroy_expr_expr_pair);
  insert_expr_expr_hash_table(&environment->table, make_expr_node(make_token(IDENTIFIER, "sqrt", NULL, 0)), make_expr_node(make_token(IDENTIFIER, "sqrt", NULL, 0)), destroy_expr_expr_pair);
  insert_expr_expr_hash_table(&environment->table, make_expr_node(make_token(LESS_EQUAL, "<=", NULL, 0)), make_expr_node(make_token(LESS_EQUAL, "<=", NULL, 0)), destroy_expr_expr_pair);
  insert_expr_expr_hash_table(&environment->table, make_expr_node(make_token(GREATER, ">", NULL, 0)), make_expr_node(make_token(GREATER, ">", NULL, 0)), destroy_expr_expr_pair);
  insert_expr_expr_hash_table(&environment->table, make_expr_node(make_token(GREATER_EQUAL, ">=", NULL, 0)), make_expr_node(make_token(GREATER_EQUAL, ">=", NULL, 0)), destroy_expr_expr_pair);
  insert_expr_expr_hash_table(&environment->table, make_expr_node(make_token(IDENTIFIER, "length", NULL, 0)), make_expr_node(make_token(IDENTIFIER, "length", NULL, 0)), destroy_expr_expr_pair);

  // just add any function you want to be a primitive of the language here
  // eg. for adding fibonacci : insert_expr_expr_hash_table(&environment->table, "fib", make_expr_node...)

  the_empty_list = malloc(sizeof(struct Expr));
  the_empty_list->expr_type = EMPTY_LIST;
  the_true = malloc(sizeof(struct Expr));
  the_true->expr_type = TRUE_TYPE;
  the_true->first_value = "true";
  the_false = malloc(sizeof(struct Expr));
  the_false->expr_type = FALSE_TYPE;
  the_false->first_value = "false";

  while (1) {
    printf("\n]=> ");
    init_scanner(&s, input);
    get_expression(input);
    strcpy(s->source, input);
    scan_tokens(s);
    printf("\n;value:\n");
    print_eval_value(eval(s, environment));
  }
  return 0;
}

void make_env(env *environment) {
  env e = malloc(sizeof(struct Env));
  init_expr_expr_hash_table(&(e->table));
  e->next = NULL;
  *environment = e;
}

expr eval(Scanner s, env e) {
  int start_index = 0;
  expr exp = create_expr(s->tokens, &start_index, 0);
  expr val = eval_expr(exp, e);
  return val;
}

expr eval_expr(expr exp, env e) {
  if (!exp)
  { // for things like comments
    return NULL;
  }
  else if (is_self_evaluating(exp)) {
    return eval_self_evaluating(exp, e);
  } 
    else if (is_variable(exp)) {
    return lookup_variable_value(exp, e);
  }
   else if (is_definition(exp)) {
    return eval_definition(exp, e);
  } 
  else if (is_if(exp)) {
    return eval_if(exp, e);
  } 
  else if (is_lambda(exp)) {
    return eval_lambda(exp, e);
  } 
  else if (is_application(exp)) {
    return apply(eval_expr(operator(exp), e), list_of_values(operands(exp), e), e);
  }
  printf("returning NULL\n");
  printf("returning NULL\n");
  return NULL;
}



int is_number(expr exp) {
  return exp->expr_type == NUMBER_TYPE;
}

int is_string(expr exp) {
  return exp->expr_type == STRING_TYPE;
}
int is_null(expr exp) {
  return exp->expr_type == EMPTY_LIST;
}

int is_empty_list(expr exp) {
  return exp->expr_type == EMPTY_LIST;
}

// basic scheme functions
expr cons(expr e1, expr e2) {
  expr e = malloc(sizeof(struct Expr));
  e->first_value = e1;
  e->second_value = e2;
  e->expr_type = PAIR;
  return e;
}

expr car(expr exp) {
  if (!exp) return NULL;
  if (is_pair(exp)) return exp->first_value;
  return NULL;
}

expr cdr(expr exp) {
  if (!exp) return NULL;
  if (is_pair(exp)) return exp->second_value;
  return NULL;
}

expr cadr(expr exp) {
  return car(cdr(exp));
}

expr caddr(expr exp) {
  return car(cdr(cdr(exp)));
}

expr caadr(expr exp) {
  return car(car(cdr(exp)));
}

expr cddr(expr exp) {
  return cdr(cdr(exp));
}

expr cdadr(expr exp) {
  return cdr(car(cdr(exp)));
}

expr cadddr(expr exp) {
  return car(cdr(cdr(cdr(exp))));
}

int is_pair(expr exp) {
  if (exp->expr_type == PAIR) return 1;
  return 0;
}

int is_self_evaluating(expr exp) {
  return is_number(exp) || is_string(exp) || exp->expr_type == FALSE_TYPE || exp->expr_type == TRUE_TYPE;
}

int is_variable(expr exp) {
  switch (exp->expr_type) {
    case IDENTIFIER_TYPE:
    case PLUS_TYPE:
    case MINUS_TYPE:
    case STAR_TYPE:
    case SLASH_TYPE:
    case LESS_TYPE:
    case LESSEQUAL_TYPE:
    case GREATER_TYPE:
    case GREATEREQUAL_TYPE:
    case EQUAL_TYPE:
    case ABS_TYPE:
      return 1;
    default:
      return 0;
  }
}

expr lookup_variable_value(expr exp, env e) {
  while (e != NULL) {
    expr_expr_pairST p_val = find_in_expr_expr_hash_table(e->table, exp);
    if (p_val) {
        return p_val->value;
    }
    e = e->next;
  }
  printf("variable %s not found\n", (char*)exp->first_value);
  return NULL;
}

int is_definition(expr exp) {
  if (is_pair(exp) && car(exp)->expr_type == DEFINE_TYPE) return 1;
  return 0;
}

int is_lambda(expr exp) {
  if (is_pair(exp) && car(exp)->expr_type == LAMBDA_TYPE) return 1;
  return 0;
}

expr eval_self_evaluating(expr exp, env e) {
  return exp;
}

expr definition_variable(expr exp) { 
  if (is_variable(cadr(exp))) { 
    return cadr(exp);
  } else if (is_pair(cadr(exp))) {
    return caadr(exp);
  }
  return NULL;
}

expr make_lambda(expr args, expr body) {
  expr l = malloc(sizeof(struct Expr));
  l->expr_type = LAMBDA_TYPE;
  expr res = cons(l, cons(args, body));
  return res;
}

expr lambda_parameters(expr exp) {
  // this works الحمد لله
  return cadr(exp);
}

expr lambda_body(expr exp) {
  // this works الحمد لله
  return cddr(exp);
}

expr definition_value(expr exp) {
  if (is_variable(cadr(exp))) {
    return caddr(exp);
  } else {
    return make_lambda(cdadr(exp), cddr(exp));
  }
}

void destroy_expr_expr_pair(expr_expr_pairST p) {
  // dummy 
}

expr eval_definition(expr exp, env e) {
  insert_expr_expr_hash_table(&e->table, definition_variable(exp), eval_expr(definition_value(exp), e), destroy_expr_expr_pair);
  expr ret_val = malloc(sizeof(struct Expr));
  ret_val->expr_type = STRING_TYPE;
  ret_val->first_value = "ok";
  return ret_val;
}

expr eval_lambda(expr exp, env e) {
  return make_procedure(lambda_parameters(exp), lambda_body(exp), e);
}

expr make_procedure(expr l_params, expr l_body, env e) {
  expr pro = malloc(sizeof(struct Expr));
  pro->expr_type = PROCEDURE_TYPE;
  return cons(pro, cons(l_params, cons(l_body, cons((expr)e, the_empty_list))));
}

expr proc_body(expr exp) {
  return caddr(exp);
}

expr proc_parameters(expr exp) {
  return cadr(exp);
}

env proc_env(expr exp) {
  return (env)cadddr(exp);
}

expr first_operand(expr exp) {
  return car(exp);
}

expr rest_operands(expr exp) {
  return cdr(exp);
}

int is_last_operand(expr exp) {
  if (is_pair(exp) && is_null(cdr(exp))) return 1;
  return 0;
}

int check_if_no_operands(expr exp) {
 return is_null(exp);
}

expr list_of_values(expr exps, env e) {
  if (check_if_no_operands(exps)) {
    return the_empty_list;
  } 
  return cons(eval_expr(first_operand(exps), e), list_of_values(rest_operands(exps), e));
}

expr operator(expr exp) {
  return car(exp);
}

expr operands(expr exp) {
  return cdr(exp);
}

int is_if(expr exp) {
  return is_pair(exp) && car(exp)->expr_type == IF_TYPE;
}

expr if_predicate(expr exp) {
  return cadr(exp);
}

expr if_consequent(expr exp) {
  return caddr(exp);
}

expr if_alternative(expr exp) {
  return cadddr(exp);
}

int is_true(expr exp) {
  return exp != the_false;
}

int is_application(expr exp) {
  return is_pair(exp);
}

expr eval_if(expr exp, env e) {
  if (is_true(eval_expr(if_predicate(exp), e))) return eval_expr(if_consequent(exp), e);
  return eval_expr(if_alternative(exp), e);
}


int is_compound_proc(expr exp) {
  return (is_pair(exp) && car(exp)->expr_type == PROCEDURE_TYPE);
}

expr first_exp(expr exps) {
  return car(exps);
}

int is_last_exp(expr exps) {
  return is_null(cdr(exps));
}

expr rest_exp(expr exps) {
  return cdr(exps);
}

expr eval_sequence(expr exps, env e) {
  if (is_last_exp(exps))
  {
    return eval_expr(first_exp(exps), e);
  }
  eval_expr(first_exp(exps), e);
  return eval_sequence(rest_exp(exps), e);
}

int length(expr exp) {
  if (is_null(exp)) return 0;
  return 1 + length(cdr(exp));
}

env extend_environment(expr vars, expr vals, env base_env) {
  env new_e;
  make_env(&new_e);
  while (!is_null(vars)) {
    insert_expr_expr_hash_table(&new_e->table, car(vars), car(vals), destroy_expr_expr_pair);
    vars = cdr(vars);
    vals = cdr(vals);
  }
  new_e->next = base_env;
  return new_e;
}

double number(expr exp) {
  return *((double*)exp->first_value);
}

expr make_number(double n) {
  expr num = malloc(sizeof(struct Expr));
  num->expr_type = NUMBER_TYPE;
  double *d = malloc(sizeof(double));
  *d = n;
  num->first_value = d;
  return num;
}

expr apply(expr proc, expr arguments, env e) {
  if (is_compound_proc(proc)) {
    return eval_sequence(proc_body(proc), extend_environment(proc_parameters(proc), arguments, proc_env(proc)));
  }

  switch (proc->expr_type) {
    case PLUS_TYPE:
      {
        double sum = 0.0;
        while (!is_null(arguments)) {
          sum += number(car(arguments));
          arguments = cdr(arguments);
        }
        return make_number(sum);
      }

    case MINUS_TYPE:
      {
        if (length(arguments) == 1)
        {
          return make_number(-number(car(arguments)));
        }
        double sum = number(car(arguments));
        arguments = cdr(arguments);

        while (!is_null(arguments)) {
          sum -= number(car(arguments));
          arguments = cdr(arguments);
        }
        return make_number(sum);
      }

    case STAR_TYPE:
      {
        double product = 1.0;
        while (!is_null(arguments)) {
          product *= number(car(arguments));
          arguments = cdr(arguments);
        }
        return make_number(product);
      }

    case SLASH_TYPE:
      if (length(arguments) == 1)
      {
        return make_number(1/number(car(arguments)));
      }
      double r = number(car(arguments));
      arguments = cdr(arguments);
      while (!is_null(arguments)) {
        r /= number(car(arguments));
        arguments = cdr(arguments); 
      }
      return make_number(r);

    case LESS_TYPE:
      if (length(arguments) <= 1)
      {
        return the_true;
      }
      while(!is_null(cdr(arguments))) {
        if (number(car(arguments)) >= number(cadr(arguments)))
        {
          return the_false;
        }
        arguments = cdr(arguments);
      }
      return the_true;
    case GREATER_TYPE:
      if (length(arguments) <= 1)
      {
        return the_true;
      }
      while(!is_null(cdr(arguments))) {
        if (number(car(arguments)) <= number(cadr(arguments)))
        {
          return the_false;
        }
        arguments = cdr(arguments);
      }
      return the_true;
    case EQUAL_TYPE:
      if (length(arguments) <= 1)
      {
        return the_true;
      }
      while(!is_null(cdr(arguments))) {
        if (number(car(arguments)) != number(cadr(arguments)))
        {
          return the_false;
        }
        arguments = cdr(arguments);
      }
      return the_true;
    case GREATEREQUAL_TYPE:
      if (length(arguments) <= 1)
      {
        return the_true;
      }
      while(!is_null(cdr(arguments))) {
        if (number(car(arguments)) < number(cadr(arguments)))
        {
          return the_false;
        }
        arguments = cdr(arguments);
      }
      return the_true;
    case LESSEQUAL_TYPE:
      if (length(arguments) <= 1)
      {
        return the_true;
      }
      while(!is_null(cdr(arguments))) {
        if (number(car(arguments)) > number(cadr(arguments)))
        {
          return the_false;
        }
        arguments = cdr(arguments);
      }
      return the_true;

    default:
      if (strcmp(proc->first_value, "length") == 0)
      {
        int n = length(arguments);
        return make_number((double)n);
      } else if (strcmp(proc->first_value, "sqrt") == 0)
      {
        double x = number(car(arguments));
        return make_number(sqrt(x));
      } else if (strcmp(proc->first_value, "abs") == 0)
      {
       double x = number(car(arguments));
       return make_number(fabs(x)); 
      }
      printf("error!\n");
      exit(0);
  }

}

/**************** PRINTING FUNCTIONS ******************/

void print_eval_value(expr exp) {
  if (!exp)
  { // for comments do nothing
  } 
  else if (exp == the_false)
  {
    printf("%s\n", (char*)exp->first_value);
  }
  else if (is_number(exp)) {
    printf("%f\n", *((double*)exp->first_value));
  } 
  else if (is_string(exp)) {
    printf("%s\n", (char*)exp->first_value);
  } 
  else if (is_compound_proc(exp)) {
    printf("[compound-procedure : parameters = ");
    print_expr(proc_parameters(exp));
    printf(", body = ");
    print_expr(proc_body(exp));
    printf("]\n");
  } 
  else print_expr(exp);
}

// this function would be called only from inside an original pair.
void print_car(expr exp){
  switch (exp->expr_type) {
    case EMPTY_LIST:
      printf("()");
      break;
    case PAIR:
      printf("(");
      print_car(car(exp));
      if (!is_empty_list(cdr(exp)))
      {
        printf(" ");
      }
      print_cdr(cdr(exp));
      break;
    case NUMBER_TYPE:
      printf("%f", *((double*)exp->first_value));
      break;
    case STRING_TYPE:
      printf("%s", (char*)exp->first_value);
      break;
    default:
      printf("%s", (char*)exp->first_value);
      break;

  }
}

void print_cdr(expr exp){
  switch (exp->expr_type) {
    case EMPTY_LIST:
      printf(")");
      break;
    case PAIR:
      print_car(car(exp));
      if (!is_empty_list(cdr(exp)))
      {
        printf(" ");
      }
      print_cdr(cdr(exp));
      break;
    case NUMBER_TYPE:
      printf("%f", *((double*)exp->first_value));
      break;
    case STRING_TYPE:
      printf("%s", (char*)exp->first_value);
      break;
    default:
      printf("%s", (char*)exp->first_value);
      break;
  }
}
void print_expr(expr exp) {
  switch (exp->expr_type) {
    case PAIR:
      printf("(");

      print_car(car(exp));
      if (!is_empty_list(cdr(exp)))
      {
        printf(" ");
      }
      print_cdr(cdr(exp));
      printf("\n");
      break;
    case NUMBER_TYPE:
      printf("%f", *((double*)exp->first_value));
      break;
    case STRING_TYPE:
      printf("%s", (char*)exp->first_value);
      break;
    case EMPTY_LIST:
      printf("()\n");
      break;
    default:
      printf("%s", (char*)exp->first_value);
      break;
  }
}


/*************** PARSING FUNCTIONS*****************/

expr create_expr(Token_array t_arr, int *start_index, int in_pair) {

  // creates the expr using the user input, essentially a parsing step
  expr e;
  Token t;

  t = t_arr->arr[*start_index]; // token at position *start_index

  if (t->type == _EOF_) return NULL;

  switch (in_pair) {
    case 0:
      if (t->type == LEFT_PAREN) {
        e = cons(NULL, NULL);
        e->expr_type = PAIR;
        *start_index = *start_index + 1;
        expr e1, e2;
        e1 = create_expr(t_arr, start_index, 0);
        e->first_value = e1;
        if (t_arr->arr[*start_index]->type == RIGHT_PAREN)
        {
          e->second_value = the_empty_list;
          *start_index = *start_index + 1;
          return e;
        }
        e2 = create_expr(t_arr, start_index, 1);
        e->second_value = e2;
        return e;
      }
      // singletons
      if (t->type == TRUE)
      {
        *start_index = *start_index + 1;
        return the_true;
      } else if (t->type == FALSE)
      {
        *start_index = *start_index + 1;
        return the_false;
      }

      // else if it's a simple token
      e = make_expr_node(t);
      *start_index = *start_index + 1;
      return e;
      break;

    case 1:
      // if we are in a list, we will first make a cons cell and repeat the above case without incrementing *start_index
      e = cons(NULL, NULL);
      e->expr_type = PAIR;
      expr e1, e2;
      e1 = create_expr(t_arr, start_index, 0); // should take care of nested lists and update start_index as needed
      e->first_value = e1;

      if (t_arr->arr[*start_index]->type == RIGHT_PAREN)
      {
          e->second_value = the_empty_list;
          *start_index = *start_index + 1;
          return e;
      }

      // singletons
      if (t->type == TRUE)
      {
        *start_index = *start_index + 1;
        return the_true;
      } else if (t->type == FALSE)
      {
        *start_index = *start_index + 1;
        return the_false;
      }
      // else if it's a simple token
      e2 = create_expr(t_arr, start_index, 1);
      e->second_value = e2;
      return e; // return the 'cons cell' made at the beginning of this case

    default:
      printf("Error in create_expr\n");
      exit(1);
  }
}


expr make_expr_node(Token t) { // converts a single token to an expr 'node'
  expr e = malloc(sizeof(struct Expr));
  switch (t->type) {
    case NUMBER:
      e->expr_type = NUMBER_TYPE;
      e->first_value = t->literal;
      break;
    case STRING:
      e->expr_type = STRING_TYPE;
      e->first_value = t->literal;
      break;
    case IDENTIFIER:
      e->expr_type = IDENTIFIER_TYPE;
      e->first_value = t->lexeme;
      break;
    case DEFINE:
      e->expr_type = DEFINE_TYPE;
      e->first_value = t->lexeme;
      break;
    case LAMBDA:
      e->expr_type = LAMBDA_TYPE;
      e->first_value = t->lexeme;
      break;
    case IF:
      e->expr_type = IF_TYPE;
      e->first_value = t->lexeme;
      break;
    case BEGIN:
      e->expr_type = BEGIN_TYPE;
      e->first_value = t->lexeme;
      break;
    case AND:
      e->expr_type = AND_TYPE;
      e->first_value = t->lexeme;
      break;
    case NOT:
      e->expr_type = NOT_TYPE;
      e->first_value = t->lexeme;
      break;
    case OR:
      e->expr_type = OR_TYPE;
      e->first_value = t->lexeme;
      break;
    case ELSE:
      e->expr_type = ELSE_TYPE;
      e->first_value = t->lexeme;
      break;
    case COND:
      e->expr_type = COND_TYPE;
      e->first_value = t->lexeme;
      break;
    case PLUS:
      e->expr_type = PLUS_TYPE;
      e->first_value = t->lexeme;
      break;
    case MINUS:
      e->expr_type = MINUS_TYPE;
      e->first_value = t->lexeme;
      break;
    case STAR:
      e->expr_type = STAR_TYPE;
      e->first_value = t->lexeme;
      break;
    case SLASH:
      e->expr_type = SLASH_TYPE;
      e->first_value = t->lexeme;
      break;
    case EQUAL:
      e->expr_type = EQUAL_TYPE;
      e->first_value = t->lexeme;
      break;
    case LESS:
      e->expr_type = LESS_TYPE;
      e->first_value = t->lexeme;
      break;
    case LESS_EQUAL:
      e->expr_type = LESSEQUAL_TYPE;
      e->first_value = t->lexeme;
      break;
    case GREATER_EQUAL:
      e->expr_type = GREATEREQUAL_TYPE;
      e->first_value = t->lexeme;
      break;
    case GREATER:
      e->expr_type = GREATER_TYPE;
      e->first_value = t->lexeme;
      break;
    default:
      printf("This should run on error\n");
      exit(0);
      break;
  }

  return e;
}
