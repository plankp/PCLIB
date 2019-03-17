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

#ifndef __BIDI_LIST_H__
#define __BIDI_LIST_H__

#include <stddef.h>
#include <stdbool.h>

typedef struct bidi_entry
{
  struct bidi_entry * prev;
  struct bidi_entry * next;
  char data[];
} bidi_entry;

typedef struct bidi_list
{
  size_t len;
  size_t blk;
  bidi_entry * first;
  bidi_entry * last;
} bidi_list;

bool init_list                (bidi_list * list, size_t data_size);

void free_list                (bidi_list * list);

void list_clear               (bidi_list * const list);

void list_reverse             (bidi_list * const list);

void list_foreach             (bidi_list const * const list, void (* it)(void const *));

void list_reveach             (bidi_list const * const list, void (* it)(void const *));

size_t list_size              (bidi_list const * const list);

bool list_add_first           (bidi_list * restrict const list, void const * restrict el);

bool list_add_last            (bidi_list * restrict const list, void const * restrict el);

bool list_remove_first        (bidi_list * restrict const list, void * restrict out);

bool list_remove_last         (bidi_list * restrict const list, void * restrict out);

void const * list_peek_first  (bidi_list const * const list);

void const * list_peek_last   (bidi_list const * const list);

#endif