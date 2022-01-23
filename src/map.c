#include "include/map.h"
#include "include/utill.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

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

Value *array_val(Value **array, size_t length)
{
  Value *val = (Value *)allocate(sizeof(Value), "Map Value");

  val->array_value = calloc(length, sizeof(Value *));
  val->array_value = array;
  val->size = length;
  return val;
}

Value *map_val(HashMap *map)
{
  Value *val = (Value *)allocate(sizeof(Value), "Map Value");
  val->object = map;
  return val;
}


Value **create_array(size_t length, ...)
{
  va_list args;
  va_start(args, length);

  Value **array = calloc(length, sizeof(Value *));
  OTM(array, array);

  for (size_t i = 0; i < length; i++)
  {
	array[i] = va_arg(args, Value *);
  }

  va_end(args);
  return array;
} 

Value *free_value(Value *value)
{
  if (value == NULL)
  {
	return NULL;
  }

  if (value->type == STRING_VAL)
  {
	free(value->string_value);
  }
  else if (value->type == ARRAY)
  {
	for (size_t i = 0; i < value->size; i++)
	{
	  free_value(value->array_value[i]);
	}
	free(value->array_value);
  }

  free(value);
  return NULL;
}


Value *new_value(Type type, void *value)
{
  Value *val = (Value *)allocate(sizeof(Value), "Map Value");
  val->type = type;

  switch (type)
  {
	case STRING_VAL:
		free_value(val);
		return string_val((char *)value);
		break;
	case INT_VAL:
	  val->int_value = *(int *)value;
	  break;
	case FLOAT_VAL:
	  val->float_value = *(float *)value;
	  break;
	case ARRAY:
	  val->array_value = value;
	  break;
	case OBJECT:
	  val->object = value;
	  break;

	default:
	  val->user_t = value;
  }

  return val;
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
  for (size_t i = 0; i < map->capacity; i++)
  {
	free((void *)map->entries[i].key);
	free_value(map->entries[i].value);
  }

  free(map->entries);
  free(map);
}

#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

static uint64_t hash_key(const char *key)
{
	uint64_t hash = FNV_OFFSET;
  	for (const char *p = key; *p != '\0'; p++)
	{
		hash ^= (uint64_t)(unsigned char)*p;
		hash *= FNV_PRIME;
	}

	return hash;
}

Value *map_get(HashMap *map, const char *key)
{
	uint64_t hash = hash_key(key);
	size_t index = (size_t)(hash & (uint64_t)(map->capacity - 1));

	while (map->entries[index].key != NULL)
	{
		if (strcmp(map->entries[index].key, key) == 0)
		{
			return map->entries[index].value;
		}

		index++;
		if (index >= map->capacity)
		{
			index = 0;
		}
	}

	return NULL;
}

const char *map_set(HashMap *table, const char *key, Value *value)
{
	assert(value != NULL);
	if (value == NULL)
	{
		return NULL;
	}

	if (table->length >= table->capacity / 2)
	{
		if (!map_expand(table))
		{
			return NULL;
		}
	}

	return map_set_entry(table->entries, table->capacity, key, value, &table->length);
}

const char *map_set_entry(MapEntry *entries, size_t capacity, const char *key, Value *value, size_t *length)
{
	uint64_t hash = hash_key(key);
	size_t index = (size_t)(hash & (uint64_t)(capacity - 1));

	while (entries[index].key != NULL)
	{
		if (strcmp(entries[index].key, key) == 0)
		{
			entries[index].value = value;
			return entries[index].key;
		}

		index++;
		if (index >= capacity)
		{
			index = 0;
		}
	}

	if (length != NULL)
	{
		key = strdup(key);
		if (key == NULL)
		{
			return NULL;
		}
		(*length)++;
	}


	entries[index].key = (char *)key;
	entries[index].value = value;

	return key;
}

bool map_expand(HashMap *map)
{
	size_t new_capacity = map->capacity * 2;
	MapEntry *new_entries = calloc(new_capacity, sizeof(MapEntry));
	if (new_entries == NULL)
	{
		return false;
	}

	for (size_t i = 0; i < map->capacity; i++)
	{
		if (map->entries[i].key != NULL)
		{
			map_set_entry(new_entries, new_capacity, map->entries[i].key, map->entries[i].value, NULL);
		}
	}

	free(map->entries);
	map->entries = new_entries;
	map->capacity = new_capacity;

	return true;
}

size_t map_size(HashMap *map)
{
	return map->length;
}

Iterator new_map_iterator(HashMap *map)
{
	Iterator it;
	it.__table = map;
	it.idx = 0;

	return it;
}

bool iter_next(Iterator *iter)
{
	HashMap *map = iter->__table;
	while (iter->idx < map->capacity)
	{
		if (map->entries[iter->idx].key != NULL)
		{
			iter->key = map->entries[iter->idx].key;
			iter->value = map->entries[iter->idx].value;
			iter->idx++;
			return true;
		}

		iter->idx++;
	}
	return false;
}

