#include "include/utill.h"
#include <stdlib.h>
#include <stdio.h>

void *allocate(size_t bytes, char *unit)
{
  void *value = malloc(bytes);
  if (!value)
  {
    fprintf(stderr, "(%s) Out of memory", unit);
    exit(-1);
  }

  return value;
}
char *read_file(char *file_path)
{
  char *contents;
  size_t size;
  FILE *fp = fopen(file_path, "r");

  if (fp == NULL)
  {
    fprintf(stderr, "File %s does not exist", file_path);
    exit(-1);
  }

  fseek(fp, 0, SEEK_END);
  size = ftell(fp);
  rewind(fp);

  printf("Size %ld\n", size);
  contents = malloc(size);
  fread(contents, size, 1, fp);

  return contents;
}
