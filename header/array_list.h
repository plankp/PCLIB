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

#ifndef __ARRAY_LIST_H__
#define __ARRAY_LIST_H__

#include <stddef.h>
#include <stdbool.h>

typedef struct array_list
{
  size_t len;
  size_t cap;
  size_t blk;
  char * mem;
} array_list;

/**
 * Initializes an array list with storage slots being able to store at least a
 * data size.
 *
 * @param list - Pointer to an uninitialized array list
 * @param data_size - Minimum capacity of each storage slot
 *
 * @return true if data_size was at least 1
 */
bool init_arrlist             (array_list * const list, size_t data_size);

/**
 * Frees an array list, making it the same as uninitialized.
 *
 * @param list - Pointer to initialized array list
 */
void free_arrlist             (array_list * const list);

/**
 * Clears the array list by setting the size to zero. To release the
 * underlying storage slots, call arrlist_compact immediately after.
 *
 * @param list - Pointer to initialized array list
 */
void arrlist_clear            (array_list * const list);

/**
 * Attempts to release unused underlying storage slots; in other words, tries
 * to make the capacity of the list the same as the size of the list.
 *
 * @param list - Pointer to initialized array list
 */
void arrlist_compact          (array_list * const list);

/**
 * Ensures the capacity is at least n.
 *
 * @param list - Pointer to initialized array list
 * @param n - New minimum capacity of the list
 *
 * @return true if capacity was already at least n or storage slots were able
 * to be allocated successfully
 */
bool arrlist_ensure_capacity  (array_list * const list, size_t n);

/**
 * Adds an item to the end of the list. Behaviour is undefined if the item
 * being added is bigger than the data size defined during list
 * initialization.
 *
 * @param list - Pointer to initialized array list
 * @param el - Pointer to the item being added
 *
 * @return true if item was successfully added
 */
bool arrlist_add              (array_list * restrict const list,
                               void * restrict el);

/**
 * Inserts an item to a specific index of the list. Behaviour is undefined if
 * the item being inserted is bigger than the data size defined during list
 * initialization.
 *
 * @param list - Pointer to initialized array list
 * @param index - Zero-based index where the insertion will take place
 * @param el - Pointer to the item being inserted
 *
 * @return true if item was successfully inserted
 */
bool arrlist_insert           (array_list * restrict const list,
                               size_t index,
                               void * restrict el);

/**
 * Replaces an item at a specific index of the list. Behaviour is undefined if
 * the item replacing is bigger than the data size defined during list
 * initialization.
 *
 * @param list - Pointer to initialized array list
 * @param index - Zero-based index where the replacement will take place
 * @param el - Pointer to the item replacing
 * @param out - Pointer that will be filled with the old value; ignored if NULL
 *
 * @return true if item was successfully replaced
 */
bool arrlist_set              (array_list * restrict const list,
                               size_t index,
                               void * restrict el,
                               void * restrict out);

/**
 * Removes an item at a specific index of the list. If there are items after
 * the index, they are shifted towards the head of the list by one.
 *
 * @param list - Pointer to initialized array list
 * @param index - Zero-based index where the removal will take place
 * @param out - Pointer that will be filled with the removed value; ignored if
 *              NULL
 *
 * @return true if item was successfully removed
 */
bool arrlist_remove           (array_list * restrict const list,
                               size_t index,
                               void * restrict out);

/**
 * Iterates through every item of the list. The state of the list, apart from
 * the items stored in the list should be kept consistent during the iteration
 * process.
 *
 * @param list - Pointer to initialized array list
 * @param it - An action to be performed on each item
 */
void arrlist_foreach          (array_list * const list,
                               void (* it)(void const *));

/**
 * Returns the pointer to the item located at the specified index.
 *
 * @param list - Pointer to initialized array list
 * @param index - Zero-based index of the item
 *
 * @return Pointer to the item, NULL if index specified was out of bounds
 */
void const * arrlist_get      (array_list * const list, size_t index);

/**
 * Returns the size of the array list
 *
 * @param list - Pointer to initialized array list
 *
 * @return size of the array list
 */
size_t arrlist_size           (array_list * const list);

/**
 * Returns the capacity of the array list
 *
 * @param list - Pointer to initialized array list
 *
 * @return capacity of the array list
 */
size_t arrlist_capacity       (array_list * const list);

/**
 * (same as arrlist_add)
 */
bool arrlist_push             (array_list * restrict const list,
                               void * restrict el);

/**
 * Removes the last item from the list.
 *
 * @param list - Pointer to initialized array list
 * @param out - Pointer that will be filled with the removed value; ignored if
 *              NULL
 *
 * @return true if item was successfully removed
 */
bool arrlist_pop              (array_list * restrict const list,
                               void * restrict out);

#endif