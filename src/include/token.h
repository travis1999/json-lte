#ifndef TOKEN_H
#define TOKEN_H
#include <stdlib.h>
#include <stdint.h>


typedef enum token_t {INT, FLOAT, STRING, LBRACKET,
                        RBRACKET, SQLEFT, SQRIGHT,
                        COMMA, COLON} TokenType;


typedef struct token_type
{
  char *value;
  TokenType type;
  struct token_type *next;
} Token;


typedef struct list_t
{
  Token *head;
  Token *last;
  size_t size;
} List;



Token *new_token(char *value, TokenType type);
void free_token(Token *token);
Token *build_string(char *source, size_t start, size_t *inc_count);
Token *build_integer(char *source, size_t start, size_t *inc_count);
void print_type(TokenType type);
void print_token(Token *token);
List *tokenize(char *source);

#endif
