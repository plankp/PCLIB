/* 
 * Copyright (c) 2019 Paul Teng
 * 
 * PCLIB is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU Lesser General Public License as   
 * published by the Free Software Foundation, version 3.
 *
 * PCLIB is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * Lesser General Lesser Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "array_list.h"

#include <stdlib.h>
#include <string.h>

bool init_arrlist
(array_list * const list, size_t data_size)
{
  if (data_size < 1)
  {
    return false;
  }

  list->len = 0;
  list->cap = 0;
  list->blk = data_size;
  list->mem = NULL;
  return true;
}

void free_arrlist
(array_list * const list)
{
  if (list->cap > 0)
  {
    free(list->mem);

    list->len = 0;
    list->cap = 0;
    list->mem = NULL;
  }
}

void arrlist_clear
(array_list * const list)
{
  list->len = 0;
}

void arrlist_compact
(array_list * const list)
{
  size_t len = list->len;
  if (list->cap == len)
  {
    /* already compact */
    return;
  }

  if (len == 0)
  {
    /* equivalent operation as free_arrlist */
    free_arrlist(list);
    return;
  }

  char * new_mem = realloc(list->mem, len * list->blk);
  if (new_mem != NULL)
  {
    /* update cap if memory is resized */
    list->cap = len;
    list->mem = new_mem;
  }

  /* if realloc failed, original memory is still usable */
}

bool arrlist_ensure_capacity
(array_list * const list, size_t n)
{
  if (list->cap >= n)
  {
    return true;
  }

  /* resize list to at least n */
  size_t new_cap = ((n / 16) + 1) * 16;
  char * new_mem = realloc(list->mem, new_cap * list->blk);
  if (new_mem == NULL)
  {
    return false;
  }

  list->cap = new_cap;
  list->mem = new_mem;
  return true;
}

bool arrlist_add
(array_list * restrict const list, void * restrict el)
{
  if (!arrlist_ensure_capacity(list, list->len + 1))
  {
    return false;
  }

  memcpy(&list->mem[list->len * list->blk], el, list->blk);
  ++list->len;
  return true;
}

bool arrlist_insert
(array_list * restrict const list, size_t index, void * restrict el)
{
  size_t len = list->len;
  if (index > len)
  {
    /* out of bounds */
    return false;
  }

  if (index == len)
  {
    return arrlist_add(list, el);
  }

  if (!arrlist_ensure_capacity(list, list->len + 1))
  {
    return false;
  }

  /* shift the indicies after towards end */
  char * offset = &list->mem[index * list->blk];
  memmove(offset + list->blk, offset, (list->len - index) * list->blk);
  memcpy(offset, el, list->blk);
  ++list->len;
  return true;
}

bool arrlist_set
(array_list * restrict const list, size_t index, void * restrict el, void * restrict out)
{
  if (index < list->len)
  {
    char * offset = &list->mem[index * list->blk];
    if (out != NULL)
    {
      memmove(out, offset, list->blk);
    }
    memmove(offset, el, list->blk);
    return true;
  }

  /* out of bounds, nothing is set */
  return false;
}

bool arrlist_remove
(array_list * restrict const list, size_t index, void * restrict out)
{
  if (index < list->len)
  {
    /* shift the indicies after towards head */
    char * offset = &list->mem[index * list->blk];
    if (out != NULL)
    {
      memmove(out, offset, list->blk);
    }
    memmove(offset, offset + list->blk, (list->len - index + 1) * list->blk);
    --list->len;
    return true;
  }

  /* out of bounds, nothing is removed */
  return false;
}

void arrlist_foreach
(array_list * const list, void (* it)(void const *))
{
  for (size_t i = 0; i < list->len; ++i)
  {
    it(&list->mem[i * list->blk]);
  }
}

void const * arrlist_get
(array_list * const list, size_t index)
{
  if (index < list->len)
  {
    return &list->mem[index * list->blk];
  }

  return NULL;
}

size_t arrlist_size
(array_list * const list)
{
  return list->len;
}

size_t arrlist_capacity
(array_list * const list)
{
  return list->cap;
}

bool arrlist_push
(array_list * restrict const list, void * restrict el)
{
  return arrlist_add(list, el);
}

bool arrlist_pop
(array_list * restrict const list, void * restrict out)
{
  const size_t len = list->len;

  if (len == 0)
  {
    return false;
  }

  return arrlist_remove(list, len - 1, out);
}