#include "heap.h"

#include <stdio.h>

static
int cmp_int
(const void *a, const void *b)
{
  return *((const int *) a) - *((const int *) b);
}

int main
(int argc, char **argv)
{
  static int list[] =
  {
    13, 25, 5, 17, 100, 9, 11, 12, 19, 4, 15, 8, 1, 6, 36
  };

  puts("Original: ");
  for (size_t i = 0; i < sizeof list / sizeof *list; ++i)
  {
    printf("%d ", list[i]);
  }
  puts("");

  make_max_heap(list, sizeof list / sizeof *list, sizeof *list, &cmp_int);

  puts("Max-heap: ");
  for (size_t i = 0; i < sizeof list / sizeof *list; ++i)
  {
    printf("%d ", list[i]);
  }
  puts("");

  return 0;
}