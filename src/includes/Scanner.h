#ifndef SCANNER_H
#define SCANNER_H
#include <stdlib.h>
#include <string.h>
#include "../../includes/resizable_array.h"
#include "Token.h"
#include "TokenType.h"

#define MAX_IN_LEN 100

typedef struct Scanner * Scanner;
typedef char* str;

ARRAY(Token)

struct Scanner {
  str source;
  Token_array tokens;
  int line;
};
int is_at_end(Scanner s, int current) {
  return current >= strlen(s->source);
}
void init_scanner(Scanner *scanner, str source) {
  *scanner = malloc(sizeof(struct Scanner));
  (*scanner)->source = malloc(MAX_IN_LEN + 1);
  (*scanner)->line = 1;
  strcpy((*scanner)->source, source);
  init_Token_array(&(*scanner)->tokens);
}
void scan_token(Scanner s, int *current) {
  int c = s->source[*current];
  switch(c) {
    case '(':
      *current = *current + 1;
      add_Token_array(s->tokens, make_token(LEFT_PAREN, "(", NULL, s->line));
      break;
    case ')':
      *current = *current + 1;
      add_Token_array(s->tokens, make_token(RIGHT_PAREN, ")", NULL, s->line));
      break;
    case '.':
      *current = *current + 1;
      add_Token_array(s->tokens, make_token(DOT, ".", NULL, s->line));
      break;
    case '-':
      *current = *current + 1;
      add_Token_array(s->tokens, make_token(MINUS, "-", NULL, s->line));
      break;
    case '+':
      *current = *current + 1;
      add_Token_array(s->tokens, make_token(PLUS, "+", NULL, s->line));
      break;
    case '*':
      *current = *current + 1;
      add_Token_array(s->tokens, make_token(STAR, "*", NULL, s->line));
      break;
    case '/':
      *current = *current + 1;
      add_Token_array(s->tokens, make_token(SLASH, "/", NULL, s->line));
      break;
    case '=':
      *current = *current + 1;
      add_Token_array(s->tokens, make_token(EQUAL, "=", NULL, s->line));
      break;
    case '<':
      *current = *current + 1;
      if (is_at_end(s, *current)) return;
      if (s->source[*current] == '=') {
        *current = *current + 1;
        add_Token_array(s->tokens, make_token(LESS_EQUAL, "<=", NULL, s->line));
      } else {
        add_Token_array(s->tokens, make_token(LESS, "<", NULL, s->line));
      }
      break;
    case '>':
      *current = *current + 1;
      if (is_at_end(s, *current)) return;
      if (s->source[*current] == '=') {
        *current = *current + 1;
        add_Token_array(s->tokens, make_token(GREATER_EQUAL, ">=", NULL, s->line));
      } else {
        add_Token_array(s->tokens, make_token(GREATER, ">", NULL, s->line));
      }
      break;
    case ' ':
    case '\t':
    case '\r':
      *current = *current + 1;
      break;
    case '\n':
      *current = *current + 1;
      s->line++;
      break;
    case ';':
      *current = *current + 1;
      while (!is_at_end(s, *current) && s->source[*current] != '\n') {
        *current = *current + 1;
      }
      if (is_at_end(s, *current)) return;
      s->line++;
      *current = *current + 1;
      break;
    case '"':
      *current = *current + 1;
      int start = *current;
      while (s->source[*current] != '"' && !is_at_end(s, *current)) {
        if (s->source[*current] == '\n') {
          s->line++;
        }
        *current = *current + 1;
      }
      str temp_str = malloc((*current - start + 1) * sizeof(char));
      for (int i = 0; i < *current - start; i++) {
        temp_str[i] = s->source[start + i];
      }
      temp_str[*current - start] = '\0';
      *current = *current + 1;
      add_Token_array(s->tokens, make_token(STRING, temp_str, temp_str, s->line));
      free(temp_str);
      break;
    default:
      printf("no matching token\n");
      break;
  }
}
void scan_tokens(Scanner s) {
  int start = 0;
  int current = 0;
  while (!is_at_end(s, current)) {
    scan_token(s, &current);
  }
}
void free_scanner(Scanner *s) {
  free((*s)->source);
  free_Token_array(&(*s)->tokens, destroy_token);
  free(*s);
  *s = NULL;
}
void get_expression(str input) {
  int c;
  int paren_balance = 0;
  c = getchar();
  while (c == ' ' || c == '\t' || c == '\n') {
    // consume all initial white space
    c = getchar();
  }
  if (c == '(') {
    paren_balance = 1; // we have to stop taking input if the paren_balance becomes 0
    input[0] = '(';
    int i;
    for (i = 1; i < MAX_IN_LEN && (c = getchar()) != EOF && paren_balance > 0; i++) {
      if (c == '(') {
        paren_balance++;
      } else if (c == ')') {
        paren_balance--;
      } else if (c == ';') { // if the input line contains ; ignore the rest of the line
        c = getchar();
        while (c != '\n' && c != EOF) {
          c = getchar();
        }
        if (c == EOF) {
          printf("Ma' Salama\n");
          exit(0);
        }
      }
      input[i] = c;
    }
    if (c == EOF) {
      printf("Ma' Salama\n");
      exit(0);
    }
    input[i] = '\0';
    return;
  }
  else if (c == ';') {
    c = getchar();
    while (c != '\n' && c != EOF) {
      c = getchar();
    }
    if (c == EOF) {
      printf("Ma' Salama\n");
      exit(0);
    }
  } else {
    int i = 0;
    while (c != ' ' && c != '\t' && c != '\n' && c != EOF) {
      input[i++] = c;
      c = getchar();
    }
    if (c == EOF) {
      printf("\nMa' Salama\n");
      exit(0);
    }
    input[i] = '\0';
    return;
  }
}
#endif
