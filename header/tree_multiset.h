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

#ifndef __TREE_MULTISET_H__
#define __TREE_MULTISET_H__

#include <stddef.h>
#include <stdbool.h>

typedef void (tmset_it)(void const *, size_t);
typedef int (key_cmp)(void const *, void const *);

typedef struct tmset_node
{
  size_t count;
  struct tmset_node * lhs; /* less than */
  struct tmset_node * rhs; /* more than */
  char data[];
} tmset_node;

typedef struct tree_multiset
{
  size_t len;
  size_t key_blk;
  tmset_node * root;
  key_cmp * key_compare;
} tree_multiset;

/**
 * Initializes a tree multiset with the specified key comparator.
 *
 * @param tree - Pointer to an uninitialized tree multiset
 * @param key_compare - Key relational comparator
 * @param value_size - Size of each value, 0 will cause multiset-like behaviour
 *
 * @return true if key_compare is not NULL and key_size is not zero
 */
bool init_tmset                       (tree_multiset * const tree,
                                       key_cmp * key_compare,
                                       size_t key_size);

/**
 * Frees a tree multiset, making it the same as uninitialized.
 *
 * @param tree - Pointer to initialized tree multiset
 */
void free_tmset                       (tree_multiset * const tree);

/**
 * Clears a tree multiset by deallocating all nodes and setting size to zero.
 *
 * @param tree - Pointer to initialized tree multiset
 */
void tmset_clear                      (tree_multiset * const tree);

/**
 * Puts a key with corresponding value into the tree.
 *
 * @param tree - Pointer to initialized tree multiset
 * @param key - Pointer to key
 *
 * @returns true if operation succeeded
 */
bool tmset_put                        (tree_multiset * restrict const tree,
                                       void const * restrict key);

/**
 * Puts a key with corresponding value into the tree only if key does not
 * exist in the tree. If such key already exists, false is returned
 *
 * @param tree - Pointer to initialized tree multiset
 * @param key - Pointer to key
 *
 * @returns true if operation succeeded, false if key already exists or
 * memory could not be allocated
 */
bool tmset_put_if_absent              (tree_multiset * restrict const tree,
                                       void const * restrict key);

/**
 * Removes all occurrences of a key
 *
 * @param tree - Pointer to initialized tree multiset
 * @param key - Pointer to key
 *
 * @return true if such a key was found and removed
 */
bool tmset_remove                     (tree_multiset * restrict const tree,
                                       void const * restrict key);

/**
 * Removes specified occurrences of a key. If occurrences specified is more
 * than the actual amount of occurrences, only the actual amount of keys will
 * be removed and that amount is returned.
 *
 * @param tree - Pointer to initialized tree multiset
 * @param key - Pointer to key
 * @param count - Specified occurrences
 *
 * @return the actual amount of matching keys removed
 */
size_t tmset_remove_count             (tree_multiset * restrict const tree,
                                       void const * restrict key,
                                       size_t const count);

/**
 * Checks if specified key exists
 *
 * @param tree - Pointer to initialized tree multiset
 * @param key - Pointer to key
 *
 * @return true if such a key exists, false otherwise
 */
bool tmset_has_key                     (tree_multiset const * restrict const tree,
                                       void const * restrict key);

/**
 * Checks the number of keys that match of the specified key
 *
 * @param tree - Pointer to initialized tree multiset
 * @param key - Pointer to key
 *
 * @return the number of matching keys
 */
size_t tmset_count_matches             (tree_multiset const * restrict const tree,
                                       void const * restrict key);

/**
 * Iterates through every key of the tree in sorted order. The state of the
 * tree should be kept consistent during the iteration process.
 *
 * @param tree - Pointer to initialized tree set
 * @param it - An action to be performed on each entry
 */
void tmset_foreach                     (tree_multiset const * const tree,
                                       tmset_it * it);

/**
 * Iterates through every key of the tree that is greater than the specified
 * key in sorted order. The state of the tree should be kept consistent during
 * the iteration process.
 *
 * @param tree - Pointer to initialized tree set
 * @param it - An action to be performed on each entry
 * @param key - Key being compared
 */
void tmset_foreach_gt                  (tree_multiset const * restrict const tree,
                                       void const * restrict key,
                                       tmset_it * it);

/**
 * Iterates through every key of the tree that is lesser than the specified
 * key in sorted order. The state of the tree should be kept consistent during
 * the iteration process.
 *
 * @param tree - Pointer to initialized tree set
 * @param it - An action to be performed on each entry
 * @param key - Key being compared
 */
void tmset_foreach_lt                  (tree_multiset const * restrict const tree,
                                       void const * restrict key,
                                       tmset_it * it);

/**
 * Returns the size of the tree multiset
 *
 * @param tree - Pointer to initialized tree multiset
 *
 * @return size of the tree multiset
 */
size_t tmset_size                      (tree_multiset const * const tree);

#endif