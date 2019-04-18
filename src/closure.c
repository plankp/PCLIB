#include "closure.h"

#include <stdlib.h>
#include <string.h>

struct closure 
{
  void *(*fptr)(closure *, size_t, void *);
  char data[];
};

closure *new_closure
(void *(*fptr)(closure *, size_t, void *), size_t data_size)
{
  /* use calloc so data is zeroed-out */
  closure *self = calloc(1, sizeof(closure) + data_size);
  if (self == NULL) return NULL;

  self->fptr = fptr;
  return self;
}

void delete_closure
(closure *clos)
{
  /* in case new_closure ever does more than just allocating memory */
  free(clos);
}

void closure_set_data
(closure *restrict clos, const void *restrict buf, size_t data_size)
{
  memcpy(clos->data, buf, data_size);
}

void *closure_get_data
(closure *clos)
{
  return clos->data;
}

void *closure_apply
(closure *clos, size_t sz, void *argv)
{
  return clos->fptr(clos, sz, argv);
}
