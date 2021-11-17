#ifndef TOKEN_H
#define TOKEN_H
#include "TokenType.h"
typedef struct Token *Token;
struct Token {
  TokenType type;
  char *lexeme;
  void *literal; // double for numbers, string for strings
  int line;
};
Token make_token(TokenType type, char *lexeme, void *literal, int line);
void destroy_token(Token t);
#endif
