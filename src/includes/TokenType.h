#ifndef TOKEN_TYPE_H
#define TOKEN_TYPE_H
// keywords are NOT,
typedef enum { LEFT_PAREN, RIGHT_PAREN, PROCEDURE, MINUS, PLUS, SLASH, STAR, EQUAL, GREATER,
  GREATER_EQUAL, LESS, LESS_EQUAL, IDENTIFIER, SYMBOL, STRING, NUMBER, AND, NOT, ELSE, FALSE, DEFINE,
  SET, FOR, IF, COND, NIL, OR, TRUE, LAMBDA, LET, BEGIN, DOT, ABS, SQRT, LOG, _EOF_} TokenType;

#endif
