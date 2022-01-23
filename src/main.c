#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "include/token.h"
#include "include/utill.h"
#include "include/list.h"

int main()
{
  char *contents = read_file("test.json");

  List *tokens = tokenize(contents);

  print_list(tokens);
  free_list(tokens);

  free(contents);

}
