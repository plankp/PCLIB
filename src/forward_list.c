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

#include "forward_list.h"

#include <stdlib.h>
#include <string.h>

static
void free_subsequent_nodes
(forward_entry * entry)
{
  while (entry != NULL)
  {
    forward_entry * const next = entry->next;
    free(entry);
    entry = next;
  }
}

bool init_fwdlist
(forward_list * const list, size_t data_size)
{
  if (data_size == 0) return false;

  list->len = 0;
  list->blk = data_size;
  list->mem = NULL;
  return true;
}

void free_fwdlist
(forward_list * const list)
{
  fwdlist_clear(list);
}

void fwdlist_clear
(forward_list * const list)
{
  if (list->len > 0)
  {
    free_subsequent_nodes(list->mem);

    list->len = 0;
    list->mem = NULL;
  }
}

void fwdlist_foreach
(forward_list const * const list, void (* it)(void const *))
{
  forward_entry const * entry = list->mem;
  while (entry != NULL)
  {
    forward_entry const * const next = entry->next;
    it(entry->data);
    entry = next;
  }
}

size_t fwdlist_size
(forward_list const * const list)
{
  return list->len;
}

void const * fwdlist_peek
(forward_list const * const list)
{
  return list->mem == NULL ? NULL : list->mem->data;
}

bool fwdlist_push
(forward_list * restrict const list, void const * restrict el)
{
  forward_entry * new_ent = malloc(sizeof(forward_entry) + list->blk);
  if (new_ent == NULL) return false;

  memcpy(new_ent->data, el, list->blk);

  /* push in front of first node */
  new_ent->next = list->mem;
  list->mem = new_ent;
  ++list->len;
  return true;
}

bool fwdlist_pop
(forward_list * restrict const list, void * restrict out)
{
  if (list->len == 0) return false;

  /* pop first node */
  forward_entry * node = list->mem;
  list->mem = node->next;
  node->next = NULL;
  --list->len;

  if (out != NULL) memcpy(out, node->data, list->blk);

  /* free it */
  free_subsequent_nodes(node);

  return true;
}