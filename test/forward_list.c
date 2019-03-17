#include "forward_list.h"

#include <stdio.h>
#include <assert.h>
#include <stddef.h>
#include <string.h>

static
bool pred_non_alpha
(void const * ptr)
{
  signed char const ch = *(char const *) ptr;
  bool r = ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z');
  return !r;
}

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

  /* preserved forward order */

  for (size_t i = 0; i < str_len; ++i)
  {
    fwdlist_add_first(&list, hello_world + i);
  }

  assert(fwdlist_size(&list) == str_len);
  fwdlist_reverse(&list);

  for (char * ptr = output; fwdlist_size(&list) > 0; ++ptr)
  {
    fwdlist_remove_first(&list, ptr);
  }

  printf("FWD %s\n", output);

  for (size_t i = 0; i < str_len; ++i)
  {
    fwdlist_add_first(&list, hello_world + i);
  }

  assert(fwdlist_size(&list) == str_len);

  for (char * ptr = output; fwdlist_size(&list) > 0; ++ptr)
  {
    fwdlist_remove_first(&list, ptr);
  }

  printf("DONE %s\n", output);

  /* remove all the character 'l' (there are 3 of them) */

  for (size_t i = 0; i < str_len; ++i)
  {
    fwdlist_add_first(&list, hello_world + i);
  }

  char const ch_l = 'l';
  assert(fwdlist_remove_match(&list, &ch_l) == 3);
  assert(fwdlist_size(&list) == str_len - 3);

  for (char * ptr = output; fwdlist_size(&list) > 0; ++ptr)
  {
    fwdlist_remove_first(&list, ptr);
    *(ptr + 1) = 0;
  }

  printf("DONE %s\n", output);

  /* remove all non alphabetical characters (there are 3 of them) */

  for (size_t i = 0; i < str_len; ++i)
  {
    fwdlist_add_first(&list, hello_world + i);
  }

  assert(fwdlist_remove_if(&list, &pred_non_alpha) == 3);
  assert(fwdlist_size(&list) == str_len - 3);

  for (char * ptr = output; fwdlist_size(&list) > 0; ++ptr)
  {
    fwdlist_remove_first(&list, ptr);
    *(ptr + 1) = 0;
  }

  printf("DONE %s\n", output);

  free_fwdlist(&list);
  return 0;
}