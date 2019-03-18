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

void fwdlist_reverse
(forward_list * const list)
{
  forward_entry * entry = list->mem;
  forward_entry * prev = NULL;
  while (entry != NULL)
  {
    forward_entry * const next = entry->next;
    entry->next = prev;
    prev = entry;
    entry = next;
  }
  list->mem = prev;
}

static
forward_entry * merge_list
(forward_entry * split1, forward_entry * split2, int (* comp)(void const *, void const *))
{
  forward_entry * newhead = NULL;

  if (split1 == NULL) return split2;
  if (split2 == NULL) return split1;

  /* recursively merge */
  if (comp(split1->data, split2->data) <= 0)
  {
    newhead = split1;
    newhead->next = merge_list(split1->next, split2, comp);
  }
  else
  {
    newhead = split2;
    newhead->next = merge_list(split1, split2->next, comp);
  }

  return newhead;
}

static
void split_list
(forward_entry * head, forward_entry ** split1, forward_entry ** split2)
{
  /* cut list in half */
  forward_entry * slow = head;
  forward_entry * fast = head->next;

  /* fast travels by 2, slow travels by 1
   * by the time fast reaches n, slow will reach n / 2
   * if n is the end, n / 2 is the middle!
   */
  while(fast != NULL)
  {
    fast = fast->next;
    if (fast != NULL)
    {
      slow = slow->next;
      fast = fast->next;
    }
  }

  *split1 = head;
  *split2 = slow->next;
  /* spliting */
  slow->next = NULL;
}

static
void sort_helper
(forward_entry ** ref, int (* comp)(void const *, void const *))
{
  /* uses mergesort */
  forward_entry * head = *ref;

  /* empty or lists with only one element is sorted */
  if (head == NULL || head->next == NULL) return;

  /* split the list in two halves */
  forward_entry * split1;
  forward_entry * split2;
  split_list(head, &split1, &split2);

  /* recursively sort the two halves */
  sort_helper(&split1, comp);
  sort_helper(&split2, comp);

  /* merge two sorted list */
  *ref = merge_list(split1, split2, comp);
}

void fwdlist_sort
(forward_list * const list, int (* comp)(void const *, void const *))
{
  if (list->len > 1) sort_helper(&list->mem, comp);
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

void const * fwdlist_get_first
(forward_list const * const list)
{
  return list->mem == NULL ? NULL : list->mem->data;
}

bool fwdlist_add_first
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

bool fwdlist_remove_first
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

size_t fwdlist_remove_match
(forward_list * restrict const list, void const * restrict const out)
{
  size_t rem_count = 0;
  forward_entry ** entry = &list->mem;
  while (*entry != NULL)
  {
    if (memcmp((*entry)->data, out, list->blk) == 0)
    {
      /* matches: remove that node, make entry point to next node */
      forward_entry * del_node = *entry;
      *entry = del_node->next;

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

size_t fwdlist_remove_if
(forward_list * restrict const list, bool (* pred)(void const *))
{
  size_t rem_count = 0;
  forward_entry ** entry = &list->mem;
  while (*entry != NULL)
  {
    if (pred((*entry)->data))
    {
      /* matches: remove that node, make entry point to next node */
      forward_entry * del_node = *entry;
      *entry = del_node->next;

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