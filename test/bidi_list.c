#include "bidi_list.h"

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

static
int comparator
(void const * a, void const * b)
{
  signed char const ch_a = *(char const *) a;
  signed char const ch_b = *(char const *) b;
  return ch_a - ch_b;
}

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

  /* remove all the character 'l' (there are 3 of them) */

  for (size_t i = 0; i < str_len; ++i)
  {
    list_add_first(&list, hello_world + i);
  }

  char const ch_l = 'l';
  assert(list_remove_match(&list, &ch_l) == 3);
  assert(list_size(&list) == str_len - 3);

  for (char * ptr = output; list_size(&list) > 0; ++ptr)
  {
    list_remove_first(&list, ptr);
    *(ptr + 1) = 0;
  }

  printf("DONE %s\n", output);

  /* remove all non alphabetical characters (there are 3 of them) */

  for (size_t i = 0; i < str_len; ++i)
  {
    list_add_last(&list, hello_world + i);
  }

  assert(list_remove_if(&list, &pred_non_alpha) == 3);
  assert(list_size(&list) == str_len - 3);

  for (char * ptr = output; list_size(&list) > 0; ++ptr)
  {
    list_remove_last(&list, ptr);
    *(ptr + 1) = 0;
  }

  printf("DONE %s\n", output);

  /* sort all the elements */

  for (size_t i = 0; i < str_len; ++i)
  {
    list_add_last(&list, hello_world + i);
  }

  list_sort(&list, &comparator);

  for (char * ptr = output; list_size(&list) > 0; ++ptr)
  {
    list_remove_last(&list, ptr);
    *(ptr + 1) = 0;
  }

  printf("DONE %s\n", output);

  free_list(&list);
  return 0;
}