#ifndef EXPR_H
#define EXPR_H
#include "TokenType.h"
#include "Scanner.h"
typedef struct Expr *expr;
typedef struct Eval_Value * Eval_Value;
typedef struct Env *env;
typedef enum ValueType{NUMBER_VAL, STRING_VAL, LIST_VAL, FUNCTION_VAL, NIL_VAL} ValueType;

struct Eval_Value {
  ValueType type;
  void *value;
};

struct Expr { // this structure is fit for holding a single-token expression as well as much more complicated
  // actually we have combined the lexing and parsing into create_expr
  void *value;
  TokenType expr_type;
  expr next;
};

// int get_user_expr(char *s, int lim); //gets a string expression from terminal
// void print_expr(expr e); //prints the user expression
// int is_numeric(char c);
// double get_number(char* expr, int* start, int lim); //gets a number from the user string
// int is_operator(char c); //checks if the character is an operator (+ *)
expr eval(Scanner s, env e);
void print_eval_value(expr exp);
expr create_expr(Token_array t_arr, int *start_index); //creates a linked list on the lines of a LISP list to enable quick processing of the user expression string
// void free_expr(expr e); //recursively frees the Expression linked list
// expr copy_expr(expr e); //deep-copy the expression
// expr copy_single_expr(expr e);
// double eval(char* s);//the outermost eval function
// double eval_expr(expr e);//the most crucial function

// start
/* eval functions for dispatching on type*/
// enum EXPR_TYPE{NUMBER, STRING, PAIR, SYMBOL, QUOTED, ASSIGNMENT, DEFINITION, IF, LAMBDA,
//               BEGIN, COND, APPLICATION};
//
// typedef enum EXPR_TYPE EXPR_TYPE;
//
// int is_self_evaluating(expr e);
// int is_variable(expr e);
// int is_quoted(expr e);
// int is_assignment(expr e);
// int is_definition(expr e);
// int is_if(expr e);
// int is_lambda(expr e);
// int is_begin(expr e);
// int is_cond(expr e);
// int is_application(expr r);
//
//
// // auxilliary
// int is_number(expr e);// in is_self_evaluating()
// int is_string(expr e);// in is_self_evaluating()
// int is_pair(expr e);// check if it is a pair
// int is_symbol(expr e);
// int is_tagged_list(expr e, EXPR_TYPE type_tag);
// //int is_expr_paren(expr e);
// double number(expr e);
//
// // to be removed
// int is_sum(expr e);
// expr addend(expr e);
// expr augend(expr e);
// int is_product(expr e) ;
// expr multiplier(expr e);
// expr multiplicand(expr e);
// int is_expr_paren(expr e);

#endif
