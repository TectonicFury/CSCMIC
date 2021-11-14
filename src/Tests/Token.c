#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "TokenType.h"
#include "Token.h"
#include "includes/hash_table.h"

Token make_token(TokenType type, char *lexeme, char *literal, int line) {
  Token token = (Token)malloc(sizeof(struct Token));
  token->type = type;
  token->lexeme = (char*)malloc(strlen(lexeme) + 1);
  strcpy(token->lexeme, lexeme);
  if (type == STRING) {
    token->literal = (char*)malloc(strlen(literal) + 1);
    strcpy((char*)token->literal, literal);
  } else if (type == NUMBER) {
    double *num = (double*)malloc(sizeof(double));
    *num = atof(lexeme);
    token->literal = num;
  }
  return token;
}
void destroy_token(Token t) {
  if (t->type == STRING || t->type == NUMBER) {
    free(t->literal);
  }
  free(t->lexeme);
  free(t);
  printf("freed\n");
}
