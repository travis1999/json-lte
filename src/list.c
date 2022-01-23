#include "include/list.h"
#include "include/token.h"
#include "include/utill.h"

List *new_list()
{
  List *list = (List *)allocate(sizeof(List), "List");
  list->head = NULL;
  list->last = NULL;
  list->size = 0;

  return list;
}


void free_list(List *list)
{
  Token *current = list->head;
  Token *next = NULL;

  while (current != NULL)
  {
    next = current->next;
    free_token(current);
    current = next;
  }
}



void append(List *list, Token *token)
{
  if (list->head == NULL)
    list->head = token;
  else
    list->last->next = token;
  list->last = token;

  list->size += 1;
}

void print_list(List *list)
{
  Token *current = list->head;

  while (current != NULL)
  {
    print_token(current);
    current=current->next;
  }
}

