#include "include/token.h"
#include "include/utill.h"
#include "include/list.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

Token *new_token(char *value, TokenType type)
{
  Token *token = (Token *)allocate(sizeof(Token), "Token");
  token->value = (char *)allocate(strlen(value), "Token->value");
  strcpy(token->value, value);
  token->type = type;
  
  token->next = NULL;
  return token;
}


void free_token(Token *token)
{
  free(token->value);
  free(token);
}


Token *build_string(char *source, size_t start, size_t *inc_count)
{
 size_t count = start + 1;

 while (source[count] != '"')
 {
   if (source[count] == '\0')
   {
      fprintf(stderr, "Invalid '%c' string: %s\n", source[count], &source[start]);
      exit(-1);
   }
    count += 1;
 }

  source[count] = '\0';
  Token *token = new_token(&source[start + 1], STRING);
  *inc_count = count - start;
  source[count] = '"';

  return token;
}

Token *build_integer(char *source, size_t start, size_t *inc_count)
{
  int dot_count = 0;
  bool is_float = false;
  char c = source[start];
  size_t stop = start;
  char tmp;
  Token *token;

  while (((c > 47 && c < 58) || c == '.') && c != '\0')
  {
    if (c == '.')
    {
      if (dot_count == 0)
      {
        is_float = true;
        dot_count += 1;
      }
      else
      {
        fprintf(stderr, "Invalid number");
        exit(-1);
      }
    }

    stop += 1;
    c = source[stop];
  }

  tmp = source[stop];
  source[stop] = '\0';

  if (is_float)
    token = new_token(&source[start], FLOAT);
  else
    token = new_token(&source[start], INT);

  source[stop] = tmp;
  *inc_count = stop - start;

  
  return token;
}

#define VALUE(X) case(X): printf("%s", #X); break
void print_type(TokenType type)
{
  switch (type)
  {
    VALUE(INT);
    VALUE(FLOAT);
    VALUE(STRING);
    VALUE(RBRACKET);
    VALUE(LBRACKET);
    VALUE(SQLEFT);
    VALUE(SQRIGHT);
    VALUE(COLON);
    VALUE(COMMA);
  }
    
}

void print_token(Token *token)
{
  printf("value: '%s', type: ", token->value);
  print_type(token->type);
  putchar('\n');
}

List *tokenize(char *source)
{
  List *tokens = new_list();
  size_t position = 0;
  Token *token;
  size_t inc_count = 1;

  while (source[position] != '\0')
  {
    char curr = source[position];
    inc_count = 1;


    switch (curr)
    {
      case '{':
        token = new_token("{", LBRACKET);
        break;

      case '}':
        token = new_token("{", RBRACKET);
        break;

      case '[':
        token = new_token("[", SQLEFT);
        break;

      case ']':
        token = new_token("]", SQRIGHT);
        break;

      case ',':
        token = new_token(",", COMMA);
        break;

      case ':':
        token = new_token(":", COLON);
        break;

      case '"':
        token = build_string(source, position, &inc_count);
        inc_count+=1;
        break;

      default:
        if (curr > 47 && curr < 58)
        {
          token = build_integer(source, position, &inc_count);
        }

        else if (curr == ' ' || curr == '\t' || curr == '\n')  
        {
          position += inc_count;
          continue;
        }
        else
        {
          fprintf(stderr, "Unhandled Token '%c'\n", curr);
          exit(-1);
        }
        break;
    }

    append(tokens, token);
    position += inc_count;
  }

  return tokens;
}

