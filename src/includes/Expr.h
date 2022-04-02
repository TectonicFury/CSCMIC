#ifndef EXPR_H
#define EXPR_H

#include "TokenType.h"
#include "Scanner.h"

typedef struct Expr *expr;
typedef struct Env *env;

typedef enum {NUMBER_TYPE, STRING_TYPE, DEFINE_TYPE, LAMBDA_TYPE, PAIR, EMPTY_LIST, PROCEDURE_TYPE, BEGIN_TYPE, 
  IF_TYPE, COND_TYPE, LET_TYPE, SET_TYPE,
              AND_TYPE, OR_TYPE, NOT_TYPE, ELSE_TYPE, IDENTIFIER_TYPE, TRUE_TYPE, FALSE_TYPE, PLUS_TYPE, MINUS_TYPE,
              SLASH_TYPE, STAR_TYPE, ABS_TYPE, SQRT_TYPE, EQUAL_TYPE, GREATER_TYPE, LESS_TYPE, GREATEREQUAL_TYPE, LESSEQUAL_TYPE } ExprType;

struct Expr { // this structure is fit for holding a single-token expression as well as much more complicated
  ExprType expr_type;
  void *first_value; // this is the value which will be checked
  void *second_value; //only in case of pair will this pointer be dereferenced
};

expr cons(expr e1, expr e2);
expr car(expr exp);
expr cdr(expr exp);
expr cadr(expr exp);
expr caddr(expr exp);
expr caadr(expr exp);
expr cddr(expr exp);
expr cdadr(expr exp);
expr cadddr(expr exp);

double number(expr exp);
expr make_number(double n);

int length(expr exp);
int is_null(expr exp);
int is_pair(expr exp);
int is_number(expr exp);
int is_variable(expr exp);
int is_string(expr exp);
int is_keyword(expr exp);
int is_empty_list(expr exp);
int is_self_evaluating(expr exp);
int is_definition(expr exp);
expr definition_variable(expr exp);
expr eval_self_evaluating(expr exp, env e);
int is_lambda(expr exp);
expr make_lambda(expr args, expr body);
expr lambda_parameters(expr exp);
expr lambda_body(expr exp);
expr definition_value(expr exp);
expr eval_definition(expr exp, env e);
expr make_procedure(expr l_params, expr l_body, env e);
int is_if(expr exp);
expr if_predicate(expr exp);
expr if_consequent(expr exp);
expr if_alternative(expr exp);
int is_true(expr exp);
int is_application(expr exp);
expr eval_if(expr exp, env e);
expr eval_lambda(expr exp, env e);
expr proc_body(expr exp);
expr proc_parameters(expr exp);
env proc_env(expr exp);
int is_compound_proc(expr exp);
expr eval_sequence(expr exps, env e);
env extend_environment(expr vars, expr vals, env base_env);
expr apply(expr proc, expr arguments, env e);
// expr copy_expr(expr exp);
expr lookup_variable_value(expr exp, env e);
expr operator(expr exp);
expr operands(expr exp);
expr list_of_values(expr exps, env e);

expr eval(Scanner s, env e);
expr eval_expr(expr exp, env e);
void print_eval_value(expr exp);


void make_env(env *environment);
expr make_expr_node(Token t);
expr create_expr(Token_array t_arr, int *start_index, int in_pair); //creates a linked list on the lines of a LISP list to enable quick processing of the user expression string



void print_car(expr exp);
void print_cdr(expr exp);
void print_expr(expr exp);

#endif
