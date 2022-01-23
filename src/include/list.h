#ifndef LIST_H
#define LIST_H
#include "token.h"


List *new_list();
void free_list(List *list);
void append(List *list, Token *token);
void print_list(List *list);
#endif
