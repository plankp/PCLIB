#include "forward_list.h"

#include <stdio.h>
#include <assert.h>
#include <stddef.h>
#include <string.h>

int main
(int argc, char ** argv)
{
  forward_list list;
  assert(init_fwdlist(&list, sizeof(char)));

  /* we will reverse a string! */
  char output[14] = { 0 }; /* str_len + 1 */
  char const * hello_world = "Hello, World!";
  size_t const str_len = strlen(hello_world);
  printf("Reversing %s\n", hello_world);

  for (size_t i = 0; i < str_len; ++i)
  {
    fwdlist_push(&list, hello_world + i);
  }

  assert(fwdlist_size(&list) == str_len);

  char * ptr = output;
  while (fwdlist_size(&list) > 0)
  {
    fwdlist_pop(&list, ptr++);
  }

  printf("DONE %s\n", output);

  free_fwdlist(&list);
  return 0;
}