#include "ring_buffer.h"

#include <stdio.h>

static
void iterate
(const void * data)
{
  printf("%d ", *(const int *) data);
}

int main
(int argc, char **argv)
{
  ring_buffer buf;
  init_ringbuf(&buf, 5, sizeof(int));

  static int data[] = { 1, 124, 973, -54, 27, 68, 0 };
  for (int *ptr = data; *ptr; ++ptr)
  {
    ringbuf_offer(&buf, ptr);
  }

  ringbuf_foreach(&buf, &iterate);
  puts("");

  printf("current element: %d\n", *(const int *) ringbuf_peek(&buf));

  while (!ringbuf_empty(&buf))
  {
    int slot = 0;
    ringbuf_poll(&buf, &slot);
    printf("%d ", slot);
  }
  puts("");

  printf("size of empty: %zu\n", ringbuf_size(&buf));
  printf("peek on empty: %p\n", ringbuf_peek(&buf));
  printf("poll on empty: %d\n", ringbuf_poll(&buf, NULL));

  int temp = 500;
  ringbuf_offer(&buf, &temp);
  printf("size: %zu\n", ringbuf_size(&buf));

  ringbuf_foreach(&buf, &iterate);
  puts("");

  free_ringbuf(&buf);

  return 0;
}