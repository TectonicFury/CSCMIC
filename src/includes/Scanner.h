#ifndef SCANNER_H
#define SCANNER_H
#include <stdlib.h>
#include <string.h>
#include "../../includes/resizable_array.h"
#include "../../includes/hash_table.h"
#include "Token.h"
#include "TokenType.h"

#define MAX_IN_LEN 1024
#define BUFSIZE 100 //for getch ungetch
char buf[BUFSIZE];
int bufp = 0;
typedef struct Scanner * Scanner;
typedef char* str;

int getch(void);
void ungetch(int c);

void get_paren_expression(str in);
void get_symbol(str in);
void get_comment(str in);
void get_simple_expression(str in);
void get_string_expression(str in);
void get_number_token(Scanner s, int *current);
int is_digit(int c);

int getch(void) { /* get a (possibly pushed-back) character */
  return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c)    /* push character back on input */
{
    if (bufp >= BUFSIZE)
         printf("ungetch: too many characters\n");
    else
         buf[bufp++] = c;
}

int str_equal_(str s1, str s2) {
  if (strcmp(s1, s2) == 0) return 1;
  return 0;
}

unsigned int str_hash_(str s, int M) {
  // djb2
  unsigned int hash = 5381;
  int c;
  c = *s;
  while (c != '\0') {
    hash = hash * 33 + c;
    s++;
    c = *s;
  }
  return hash % M;
}

ARRAY(Token)
HashTable(str, TokenType, str_equal, str_hash) // need to add hash function for strings

struct Scanner {
  str source;
  Token_array tokens;
  int line;
  str_TokenType_hash_table keywords;
};

void destroy_str_TokenType_pair(str_TokenType_pairST p) {
  if (p) {
    // free(p->key); bus error: memory being freed was never allocated (for the keyword strings)
    free(p);
  }
}

void init_scanner(Scanner *scanner, str source) {
  *scanner = malloc(sizeof(struct Scanner));
  (*scanner)->source = malloc(MAX_IN_LEN + 1);
  (*scanner)->line = 1;
  strcpy((*scanner)->source, source);
  init_Token_array(&(*scanner)->tokens);

  // initializing keywords hash table
  init_str_TokenType_hash_table(&((*scanner)->keywords));
  insert_str_TokenType_hash_table(&((*scanner)->keywords), "and", AND, destroy_str_TokenType_pair);
  insert_str_TokenType_hash_table(&((*scanner)->keywords), "not", NOT, destroy_str_TokenType_pair);
  insert_str_TokenType_hash_table(&((*scanner)->keywords), "if", IF, destroy_str_TokenType_pair);
  insert_str_TokenType_hash_table(&((*scanner)->keywords), "cond", COND, destroy_str_TokenType_pair);
  insert_str_TokenType_hash_table(&((*scanner)->keywords), "else", ELSE, destroy_str_TokenType_pair);
  insert_str_TokenType_hash_table(&((*scanner)->keywords), "or", OR, destroy_str_TokenType_pair);
  insert_str_TokenType_hash_table(&((*scanner)->keywords), "lambda", LAMBDA, destroy_str_TokenType_pair);
  insert_str_TokenType_hash_table(&((*scanner)->keywords), "define", DEFINE, destroy_str_TokenType_pair);
  insert_str_TokenType_hash_table(&((*scanner)->keywords), "true", TRUE, destroy_str_TokenType_pair);
  insert_str_TokenType_hash_table(&((*scanner)->keywords), "false", FALSE, destroy_str_TokenType_pair);
  insert_str_TokenType_hash_table(&((*scanner)->keywords), "nil", NIL, destroy_str_TokenType_pair);
  insert_str_TokenType_hash_table(&((*scanner)->keywords), "set!", SET, destroy_str_TokenType_pair);
  insert_str_TokenType_hash_table(&(*scanner)->keywords, "let", LET, destroy_str_TokenType_pair);
  insert_str_TokenType_hash_table(&(*scanner)->keywords, "begin", BEGIN, destroy_str_TokenType_pair);
}

int is_at_end(Scanner s, int current) {
  return current >= strlen(s->source);
}

int is_digit(int c) {
  return c >= '0' && c <= '9';
}

int is_alpha(int c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '!' || c == '-';
}

int is_alpha_numeric(int c) {
  return is_digit(c) || is_alpha(c);
}

void get_number_token(Scanner s, int *current) {
  int start = *current;

  while (is_digit(s->source[*current])) {
    *current = *current + 1;
  }

  if (s->source[*current] == '.') {
    *current = *current + 1;
    while (is_digit(s->source[*current])) {
      *current = *current + 1;
    }
  }

  str num_str = malloc((*current - start + 1) * sizeof(char));
  for (int i = 0; i < *current - start; i++) {
    num_str[i] = s->source[start + i];
  }
  num_str[*current - start] = '\0';
  add_Token_array(s->tokens, make_token(NUMBER, num_str, NULL, s->line));
}

void get_identifier_token(Scanner s, int *current) {
  int start = *current;
  while (is_alpha_numeric(s->source[*current])) {
    *current = *current + 1;
  }
  str id_str = malloc(sizeof(char) * (*current - start + 1));
  for (int i = 0; i < *current - start; i++) {
    id_str[i] = s->source[start + i];
  }
  id_str[*current - start] = '\0';
  str_TokenType_pairST entry = find_in_str_TokenType_hash_table(s->keywords, id_str);
  if (entry) {
    printf("found token %s\n", id_str);
    add_Token_array(s->tokens, make_token(entry->value, id_str, NULL, s->line));
    return;
  }
  add_Token_array(s->tokens, make_token(IDENTIFIER, id_str, NULL, s->line));
}

void get_string_token(Scanner s, int *current) {
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
      get_string_token(s, current);
      break;
    default:
      if (is_digit(c)) {
        // *current = *current - 1;
        get_number_token(s, current);
      } else if (is_alpha(c)) {
        get_identifier_token(s, current);
      }
      else {
        printf("ERROR: no matching token > %c < on line %d\n", c, s->line);
        exit(0);
      }
      break;
  }
}

void scan_tokens(Scanner s) {
  // int start = 0;
  int current = 0;
  while (!is_at_end(s, current)) {
    scan_token(s, &current);
  }
}

void free_scanner(Scanner *s) {
  free((*s)->source);
  free_Token_array(&(*s)->tokens, destroy_token);
  free_str_TokenType_hash_table(&(*s)->keywords, destroy_str_TokenType_pair);
  free(*s);
  *s = NULL;
}

void get_simple_expression(str in) {
  int index = 0;
  int c = getch();
  in[index] = c;
  while (c != ' ' && c != '\n' && c != EOF) {
    c = getch();
    in[++index] = c;
  }

  if (c == EOF) {
    exit(0);
  }
  in[++index] = '\0';
}

void get_symbol(str in) {
  int index = 0;
  int c = '\'';
  in[index] = '\'';

  c = getch();
  while (c == ' ' || c == '\t' || c == '\n') {
    c = getch(); // ignore the initial white space
  }

  if (c == '(') {
    get_paren_expression(&in[1]);
    return;
  }
  // else
  in[++index] = c;
  while (c != '\n' && c != EOF) {
    c = getch();
    in[++index] = c;
  }
  if (c == EOF) {
    exit(0);
  }

  in[++index] = '\0';
}

void get_string_expression(str in) {
  int index = 0;
  int c = '"';
  in[index] = c;
  c = getch();
  while (c != '"' && c != EOF) {
    in[++index] = c;
    c = getch();
  }

  if (c == EOF) {
    exit(0);
  }

  in[++index] = '"';
  in[++index] = '\0';
}

void get_comment(str in) {
  int index = 0;
  int c = ';';
  in[index] = c;
  while (c != '\n' && c != EOF) {
    c = getch();
    in[++index] = c;
  }
  if (c == EOF) {
    exit(0);
  }
  in[++index] = '\0';
}

void get_paren_expression(str in) {
  int paren_balance = 1;
  int index = 0;
  int c = '(';
  in[index] = c;
  while (paren_balance > 0) {
    c = getch();
    switch (c) {
      case '(':
        paren_balance++;
        in[++index] = '(';
        break;
      case ')':
        paren_balance--;
        in[++index] = ')';
        break;
      case EOF:
        exit(0);
        break;
      default:
        in[++index] = c;
        break;
    }
  }
  in[++index] = '\0';
}

/* an expression can start with a
'\'' (quote),
'(',
; (for comments),
or simple [a-z] | [0-9] (simple)

Each of these expression types can contain the other types
eg. the ( can contain quotations and the quotation can contain the ()
*/

void get_expression(str input) {
  int c = getchar();
  while (c == ' ' || c == '\t' || c == '\n') {
    // consume all initial white space
    c = getchar();
  }

  switch (c) {
    case '(':
      get_paren_expression(input);
      break;
    case '"':
      get_string_expression(input);
      break;
    case ';':
      get_comment(input);
      break;
    case '\'':
      get_symbol(input);
      break;
    default:
      ungetch(c);
      get_simple_expression(input);
      break;
  }
}
#endif
