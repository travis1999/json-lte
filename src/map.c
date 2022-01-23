#include "include/map.h"
#include "include/utill.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define INITIAL_CAPACITY 16


#define OTM(X, Y)\
  if ((X) == NULL){\
    fprintf(stderr, "Out of memory");\
    free((Y));\
    exit(-1);\
  }



Value *string_val(char *value)
{
  Value *val = (Value *)allocate(sizeof(Value), "Map Value");
  size_t length = strlen(value);

  val->string_value = malloc(length + 1);
  OTM(val->string_value, val);
  
  strcpy(val->string_value, value);
  val->string_value[length] = 0;

  return val;
}

Value *int_val(int value)
{
  Value *val = (Value *)allocate(sizeof(Value), "Map Value");
  val->int_value = value;
  return val;
}

Value *float_val(float value)
{
  Value *val = (Value *)allocate(sizeof(Value), "Map Value");
  val->float_value = value;
  return val;
}










Value *new_value(Type type, void *value)
{
   switch (type)
  {
    case ANY:
      val->user_t = value;
      break;
    case STRING_VAL:
       }
}

Value *free_value(Value *value)
{
  
}

HashMap *new_hashmap(void)
{
  HashMap *map = (HashMap *)allocate(sizeof(HashMap), "Hash Table");
  map->length = 0;
  map->capacity = INITIAL_CAPACITY;
  map->entries = calloc(map->capacity, sizeof(MapEntry));
  
  if (map->entries == NULL)
  {
    free(map);
    fprintf(stderr, "Out of memory");
    exit(EXIT_FAILURE);
  }

  return map;
}

void free_hashmap(HashMap *map)
{
  
}

Value *map_get(HashMap *map, const char *key)
{

}

const char *map_set(HashMap *table, const char *key, Value *value)
{

}

size_t map_size(HashMap *map)
{

}

Iterator new_map_iterator(HashMap *map)
{

}

bool iter_next(Iterator *iter)
{

}

