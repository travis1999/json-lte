#ifndef HASH_H
#define HASH_H
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


typedef struct hash_map_t HashMap;
typedef enum type_t {ANY, INT_VAL, FLOAT_VAL, STRING_VAL, ARRAY, OBJECT} Type;

typedef struct value_t
{
 Type type; 
  union{
    int64_t int_value;
    double float_value;
    char *string_value;
    struct value_t **array_value;
    HashMap *object;
    void *user_t;
  };
  size_t size;
} Value;

typedef struct map_entry_t {
    const char* key;
    void* value;
} MapEntry;

struct hash_map_t{
    struct map_entry_t* entries;
    size_t capacity;
    size_t length;
};


HashMap *new_hashmap(void);
void free_hashmap(HashMap *map);
Value *map_get(HashMap *map, const char *key);
const char *map_set(HashMap *table, const char *key, Value *value);
size_t map_size(HashMap *map);

typedef struct iterator_t
{
  const char *key;
  Value *value;
  HashMap *__table;
  size_t idx;
} Iterator;

Iterator new_map_iterator(HashMap *map);
bool iter_next(Iterator *iter);

bool map_expand(HashMap *map);
const char *map_set_entry(MapEntry *entries, size_t capacity, const char *key, Value *value, size_t *length);


#endif
