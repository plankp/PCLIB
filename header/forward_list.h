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

#ifndef __FORWARD_LIST_H__
#define __FORWARD_LIST_H__

#include <stddef.h>
#include <stdbool.h>

typedef struct forward_entry
{
  struct forward_entry * next;
  char data[];
} forward_entry;

typedef struct forward_list
{
  size_t len;
  size_t blk;
  forward_entry * mem;
} forward_list;

/**
 * Initializes a forward (single linked) list with each slot being able to
 * store at least a data size.
 *
 * @param list - Pointer to an uninitialized forward list
 * @param data_size - Minimum capacity of each slot
 *
 * @return true if data_size was at least 1
 */
bool init_fwdlist         (forward_list * const list, size_t data_size);

/**
 * Frees a forward list, making it the same as uninitialized.
 *
 * @param list - Pointer to initialized forward list
 */
void free_fwdlist         (forward_list * const list);

/**
 * Clears the forward list
 *
 * @param list - Pointer to initialized forward list
 */
void fwdlist_clear        (forward_list * const list);

/**
 * Iterates through every item of the list. The state of the list should be
 * kept consistent during the iteration process.
 *
 * @param list - Pointer to initialized forward list
 * @param it - An action to be performed on each item
 */
void fwdlist_foreach      (forward_list const * const list,
                           void (* it)(void const *));

/**
 * Returns the size of the forward list
 *
 * @param list - Pointer to initialized forward list
 *
 * @return size of the forward list
 */
size_t fwdlist_size       (forward_list const * const list);

/**
 * Inserts a new element to the front of the forward list
 *
 * @param list - Pointer to initialized forward list
 * @param el - Element being pushed
 *
 * @return true if process was successful
 */
bool fwdlist_add_first          (forward_list * restrict const list,
                                 void const * restrict el);

/**
 * Removes an element from the front of the forward list
 *
 * @param list - Pointer to initialized forward list
 * @param out - Pointer that will be filled with the removed value; ignored if
 *              NULL
 *
 * @return true if process was successful
 */
bool fwdlist_remove_first       (forward_list * restrict const list,
                                 void * restrict out);

/**
 * Returns pointer to the data of the first element of the forward list
 *
 * @param list - Pointer to initialized forward list
 *
 * @return pointer to the data, NULL if list is empty
 */
void const * fwdlist_get_first  (forward_list const * const list);

#endif