#include "heap.h"

#include <string.h>

static
void heapify
(char *ptr, size_t count, size_t size, int (*cmp)(const void *, const void *), char *swap_buf)
{
  for (size_t i = 1; i < count; ++i)
  {
    /*
     * i = %2 (2 - %2)
     * 1 1  1    1
     * 2 2  0    2
     * 3 1  1    1
     * 4 2  0    2
     * 5 1  1    1
     */

    size_t plevel = i;
    plevel = (plevel - (2 - plevel % 2)) / 2;

    void *child = &ptr[size * i];
    void *parent = &ptr[size * plevel];

    /* outcome: parent >= children */
    while (cmp(parent, child) < 0)
    {
      /* only consider cases when parent != children */
      memcpy(swap_buf, parent, size);
      memmove(parent, child, size);
      memcpy(child, swap_buf, size);

      /* stop when we hit root already (when parent level is 0) */
      if (plevel == 0) break;

      /* now check parent against parent's parent */
      plevel = (plevel - (2 - plevel % 2)) / 2;
      child = parent;
      parent = &ptr[size * plevel];
    }
  }
}

void make_max_heap
(void *gptr, size_t count, size_t size, int (*cmp)(const void *, const void *))
{
  /* Heap:
   *   A      A [0=2n]
   *  / \     B [1=2n+1]
   * B   C    C [2=2n+2]
   *    / \   D
   *   D   E  E
   */
  char swap_buf[size];
  heapify(gptr, count, size, cmp, swap_buf);
}

void heapsort
(void *gptr, size_t count, size_t size, int (*cmp)(const void *, const void *))
{
  /* convert array to maximum heap */
  make_max_heap(gptr, count, size, cmp);

  /* shuffle the reverse of gptr */
  char *ptr = gptr;
  char swap_buf[size];

  for (size_t i = count; i > 0; )
  {
    char *slot = &ptr[--i * size];

    memcpy(swap_buf, slot, size);
    memmove(slot, ptr, size);
    memcpy(ptr, swap_buf, size);

    heapify(gptr, i, size, cmp, swap_buf);
  }
}
