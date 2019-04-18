#include "closure.h"

#include <stdio.h>

static
void *counter(closure *self, size_t sz, void *argv)
{
  int *data = closure_get_data(self);
  ++(*data);
  return data;
}


int main
(int argc, char ** argv)
{
  closure *clos_a = new_closure(&counter, sizeof(int));
  closure *clos_b = new_closure(&counter, sizeof(int));

  closure_apply(clos_a, 0, NULL);
  closure_apply(clos_a, 0, NULL);
  int *ret_a = closure_apply(clos_a, 0, NULL);
  /* a's data should be 3 */

  int *ret_b = closure_apply(clos_b, 0, NULL);
  /* b's data should be 1 */

  printf("clos a: %d\n", *ret_a);
  printf("clos b: %d\n", *ret_b);

  delete_closure(clos_a);
  delete_closure(clos_b);
}