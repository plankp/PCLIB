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
  struct bidi_entry *prev;
  struct bidi_entry *next;
  char data[];
} bidi_entry;

typedef struct bidi_list
{
  size_t len;
  size_t blk;
  bidi_entry *first;
  bidi_entry *last;
} bidi_list;

/**
 * Initializes a bidirectional (double linked) list with each slot being able
 * to store at least a data size.
 *
 * @param list - Pointer to an uninitialized bidi list
 * @param data_size - Minimum capacity of each slot
 *
 * @return true if data_siz was at least 1
 */
bool init_list                        (bidi_list *list,
                                       size_t data_size);

/**
 * Frees a bidi list, making it the same as uninitialized.
 *
 * @param list - Pointer to initialized bidi list
 */
void free_list                      (bidi_list *list);

/**
 * Clears a bidi list
 *
 * @param list - Pointer to initialized bidi list
 */
void list_clear                     (bidi_list *list);

/**
 * Reverses a bidi list
 *
 * @param list - Pointer to initialized bidi list
 */
void list_reverse                   (bidi_list *list);

/**
 * Sorts the bidi list
 *
 * @param list - Pointer to initialized bidi list
 * @param comp - Comparator
 */
void list_sort                      (bidi_list *list,
                                     int (*comp)(const void *, const void *));

/**
 * Iterates from the first to last item of the list. The state of the list
 * should be kept consistent during the iteration process.
 *
 * @param list - Pointer to initialized bidi list
 * @param it - An action to be performed on each item
 */
void list_foreach                   (const bidi_list *list,
                                     void (*it)(const void *));

/**
 * Iterates from the last to first item of the list. The state of the list
 * should be kept consistent during the iteration process.
 *
 * @param list - Pointer to initialized bidi list
 * @param it - An action to be performed on each item
 */
void list_reveach                   (const bidi_list *list,
                                     void (*it)(const void *));

/**
 * Returns the size of the bidi list
 *
 * @param list - Pointer to initialized bidi list
 *
 * @return size of the bidi list
 */
size_t list_size                    (const bidi_list *list);

/**
 * Inserts a new element to the front of the bidi list
 *
 * @param list - Pointer to initialized bidi list
 * @param el - Element being added
 *
 * @return true if process was successful
 */
bool list_add_first                 (bidi_list *restrict list,
                                     const void *restrict el);

/**
 * Inserts a new element to the back of the bidi list
 *
 * @param list - Pointer to initialized bidi list
 * @param el - Element being added
 *
 * @return true if process was successful
 */
bool list_add_last                  (bidi_list *restrict list,
                                     const void *restrict el);

/**
 * Removes an element from the front of the bidi list
 *
 * @param list - Pointer to initialized bidi list
 * @param out - Pointer that will be filled with the removed value; ignored if
 *              NULL
 *
 * @return true if process was successful
 */
bool list_remove_first              (bidi_list *restrict list,
                                     void *restrict out);

/**
 * Removes an element from the back of the bidi list
 *
 * @param list - Pointer to initialized bidi list
 * @param out - Pointer that will be filled with the removed value; ignored if
 *              NULL
 *
 * @return true if process was successful
 */
bool list_remove_last               (bidi_list *restrict list,
                                     void *restrict out);

/**
 * Returns pointer to the data of the first element of the bidi list
 *
 * @param list - Pointer to initialized bidi list
 *
 * @return pointer to the data, NULL if list is empty
 */
const void *list_get_first          (const bidi_list *list);


/**
 * Returns pointer to the data of the last element of the bidi list
 *
 * @param list - Pointer to initialized bidi list
 *
 * @return pointer to the data, NULL if list is empty
 */
const void *list_get_last           (const bidi_list *list);


/**
 * Remove all matching elements from the forward list
 *
 * @param list - Pointer to initialized forward list
 * @param el - Matching element
 *
 * @return number of elements removed
 */
size_t list_remove_match            (bidi_list *restrict list,
                                     const void *restrict out);

/**
 * Remove all matching elements from the forward list
 *
 * @param list - Pointer to initialized forward list
 * @param pred - Predicate to check if element matches
 *
 * @return number of elements removed
 */
size_t list_remove_if               (bidi_list *list,
                                     bool (*pred)(const void *));

#endif