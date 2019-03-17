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

#include "bidi_list.h"

#include <stdlib.h>
#include <string.h>

static
void free_subsequent_nodes
(bidi_entry * entry)
{
  while (entry != NULL)
  {
    bidi_entry * const next = entry->next;
    free(entry);
    entry = next;
  }
}

static
bidi_entry * make_node
(bidi_list const * restrict list, void const * restrict el)
{
  bidi_entry * new_node = malloc(sizeof(bidi_entry) + list->blk);
  if (new_node == NULL) return NULL;

  new_node->prev = NULL;
  new_node->next = NULL;
  memcpy(new_node->data, el, list->blk);

  return new_node;
}

static inline
void insert_before
(bidi_entry * const restrict spot, bidi_entry * const restrict ins)
{
  ins->next = spot; /* 1 */
  ins->prev = spot == NULL ? NULL : spot->prev; /* 2 */
  if (spot != NULL && spot->prev != NULL) spot->prev->next = ins; /* 2 */
  if (spot != NULL) spot->prev = ins; /* 1 */
}

static inline
void insert_after
(bidi_entry * const restrict spot, bidi_entry * const restrict ins)
{
  ins->prev = spot; /* 1 */
  ins->next = spot == NULL ? NULL : spot->next; /* 2 */
  if (spot != NULL && spot->next != NULL) spot->next->prev = ins; /* 2 */
  if (spot != NULL) spot->next = ins; /* 1 */
}

static inline
bidi_entry * remove_at
(bidi_entry * const spot)
{
  bidi_entry * const prev = spot->prev;
  bidi_entry * const next = spot->next;

  /* purposely keep the links from spot to nodes */

  /* unlink previous and next nodes from spot */
  if (prev != NULL) prev->next = next;
  if (next != NULL) next->prev = prev;

  return spot;
}

bool init_list
(bidi_list * list, size_t data_size)
{
  if (data_size == 0) return false;

  list->len = 0;
  list->blk = data_size;
  list->first = NULL;
  list->last = NULL;
  return true;
}

void free_list
(bidi_list * list)
{
  list_clear(list);
}

void list_clear
(bidi_list * const list)
{
  if (list->len > 0)
  {
    free_subsequent_nodes(list->first);

    list->len = 0;
    list->first = NULL;
    list->last = NULL;
  }
}

void list_reverse
(bidi_list * const list)
{
  bidi_entry * entry = list->first;
  while (entry != NULL)
  {
    bidi_entry * const next = entry->next;
    /* swap next and prev pointers of each node */
    entry->next = entry->prev;
    entry->prev = next;
    entry = next;
  }

  /* then swap first and last pointers */
  bidi_entry * const temp = list->first;
  list->first = list->last;
  list->last = temp;
}

void list_foreach
(bidi_list const * const list, void (* it)(void const *))
{
  bidi_entry const * entry = list->first;
  while (entry != NULL)
  {
    bidi_entry const * const next = entry->next;
    it(entry->data);
    entry = next;
  }
}

void list_reveach
(bidi_list const * const list, void (* it)(void const *))
{
  bidi_entry const * entry = list->last;
  while (entry != NULL)
  {
    bidi_entry const * const next = entry->prev;
    it(entry->data);
    entry = next;
  }
}

size_t list_size
(bidi_list const * const list)
{
  return list->len;
}

bool list_add_first
(bidi_list * restrict const list, void const * restrict el)
{
  bidi_entry * const new_ent = make_node(list, el);
  if (new_ent == NULL) return false;

  /* add before first node */
  insert_before(list->first, new_ent);
  /* make the new entry the first element */
  list->first = new_ent;
  if (list->len++ == 0) list->last = new_ent;
  return true;
}

bool list_add_last
(bidi_list * restrict const list, void const * restrict el)
{
  bidi_entry * const new_ent = make_node(list, el);
  if (new_ent == NULL) return false;

  /* add after last node */
  insert_after(list->last, new_ent);
  /* make the new entry the last element */
  list->last = new_ent;
  if (list->len++ == 0) list->first = new_ent;
  return true;
}

bool list_remove_first
(bidi_list * restrict const list, void * restrict out)
{
  if (list->len == 0) return false;

  /* remove first node */
  bidi_entry * const rem = remove_at(list->first);
  /* make node after the removed one the first element */
  list->first = rem->next;
  if (--list->len == 0) list->last = NULL;

  /* unlink node from list */
  rem->prev = NULL;
  rem->next = NULL;

  if (out != NULL) memcpy(out, rem->data, list->blk);

  /* free it */
  free_subsequent_nodes(rem);

  return true;
}

bool list_remove_last
(bidi_list * restrict const list, void * restrict out)
{
  if (list->len == 0) return false;

  /* remove last node */
  bidi_entry * const rem = remove_at(list->last);
  /* make node before the removed one the last element */
  list->last = rem->prev;
  if (--list->len == 0) list->first = NULL;

  /* unlink node from list */
  rem->prev = NULL;
  rem->next = NULL;

  if (out != NULL) memcpy(out, rem->data, list->blk);

  /* free it */
  free_subsequent_nodes(rem);

  return true;
}

void const * list_get_first
(bidi_list const * const list)
{
  return list->first == NULL ? NULL : list->first->data;
}

void const * list_get_last
(bidi_list const * const list)
{
  return list->last == NULL ? NULL : list->last->data;
}

size_t list_remove_match
(bidi_list * restrict const list, void const * restrict const out)
{
  size_t rem_count = 0;
  bidi_entry ** entry = &list->first;
  while (*entry != NULL)
  {
    if (memcmp((*entry)->data, out, list->blk) == 0)
    {
      /* matches: remove that node, make entry point to next node */
      bidi_entry * del_node = *entry;
      remove_at(del_node);
      if (del_node == list->last) list->last = del_node->prev;
      *entry = del_node->next;

      del_node->prev = NULL;
      del_node->next = NULL;
      free_subsequent_nodes(del_node);

      ++rem_count;
    }
    else
    {
      /* move to next node */
      entry = &(*entry)->next;
    }
  }
  list->len -= rem_count;
  return rem_count;
}

size_t list_remove_if
(bidi_list * restrict const list, bool (* pred)(void const *))
{
  size_t rem_count = 0;
  bidi_entry ** entry = &list->first;
  while (*entry != NULL)
  {
    if (pred((*entry)->data))
    {
      /* matches: remove that node, make entry point to next node */
      bidi_entry * del_node = *entry;
      remove_at(del_node);
      if (del_node == list->last) list->last = del_node->prev;
      *entry = del_node->next;

      del_node->prev = NULL;
      del_node->next = NULL;
      free_subsequent_nodes(del_node);

      ++rem_count;
    }
    else
    {
      /* move to next node */
      entry = &(*entry)->next;
    }
  }
  list->len -= rem_count;
  return rem_count;
}