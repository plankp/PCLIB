#include "bidi_list.h"

#include <stdio.h>
#include <assert.h>

#include <stdio.h>
#include <assert.h>
#include <stddef.h>
#include <string.h>

int main
(int argc, char ** argv)
{
  bidi_list list;
  assert(init_list(&list, sizeof(char)));

  /* we will reverse a string! */
  char output[14] = { 0 }; /* str_len + 1 */
  char const * hello_world = "Hello, World!";
  size_t const str_len = strlen(hello_world);
  printf("STRING: %s\n", hello_world);

  // add last then remove last

  for (size_t i = 0; i < str_len; ++i)
  {
    list_add_last(&list, hello_world + i);
  }

  assert(list_size(&list) == str_len);

  for (char * ptr = output; list_size(&list) > 0; ++ptr)
  {
    list_remove_last(&list, ptr);
  }

  printf("REV %s\n", output);

  // add first then remove first

  for (size_t i = 0; i < str_len; ++i)
  {
    list_add_first(&list, hello_world + i);
  }

  assert(list_size(&list) == str_len);

  for (char * ptr = output; list_size(&list) > 0; ++ptr)
  {
    list_remove_first(&list, ptr);
  }

  printf("REV %s\n", output);

  // add last, reverse then remove first

  for (size_t i = 0; i < str_len; ++i)
  {
    list_add_last(&list, hello_world + i);
  }

  list_reverse(&list);

  assert(list_size(&list) == str_len);

  for (char * ptr = output; list_size(&list) > 0; ++ptr)
  {
    list_remove_first(&list, ptr);
  }

  printf("REV %s\n", output);

  // add first, reverse then remove last

  for (size_t i = 0; i < str_len; ++i)
  {
    list_add_first(&list, hello_world + i);
  }

  list_reverse(&list);

  assert(list_size(&list) == str_len);

  for (char * ptr = output; list_size(&list) > 0; ++ptr)
  {
    list_remove_last(&list, ptr);
  }

  // add last then remove first

  for (size_t i = 0; i < str_len; ++i)
  {
    list_add_last(&list, hello_world + i);
  }

  assert(list_size(&list) == str_len);

  for (char * ptr = output; list_size(&list) > 0; ++ptr)
  {
    list_remove_first(&list, ptr);
  }

  printf("FWD %s\n", output);

  // add first then remove last

  for (size_t i = 0; i < str_len; ++i)
  {
    list_add_first(&list, hello_world + i);
  }

  assert(list_size(&list) == str_len);

  for (char * ptr = output; list_size(&list) > 0; ++ptr)
  {
    list_remove_last(&list, ptr);
  }

  printf("FWD %s\n", output);

  free_list(&list);
  return 0;
}