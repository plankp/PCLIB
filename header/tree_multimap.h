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

#ifndef __TREE_MULTIMAP_H__
#define __TREE_MULTIMAP_H__

#include <stddef.h>
#include <stdbool.h>

/**
 * The growth function for each node of the multimap.
 *
 * By default, the capacity is unscaled (no elements are preallocated)
 *
 * @param n - The precomputed new capacity
 *
 * @return value >= n
 */
#ifndef TMMAP_GROW /* (size_t n) */
#define TMMAP_GROW(n) (n)
#endif

typedef void (tmmap_it)(const void *, size_t, const void *);
typedef int (key_cmp)(const void *, const void *);

typedef struct tmmap_node
{
  size_t count;
  size_t cap;
  struct tmmap_node *lhs; /* less than */
  struct tmmap_node *rhs; /* more than */
  char data[];
} tmmap_node;

typedef struct tree_multimap
{
  size_t len;
  size_t key_blk;
  size_t value_blk;
  tmmap_node *root;
  key_cmp *key_compare;
} tree_multimap;

/**
 * Initializes a tree multimap with the specified key comparator and value
 * size.
 *
 * @param tree - Pointer to an uninitialized tree multimap
 * @param key_compare - Key relational comparator
 * @param key_size - Size of each key
 * @param value_size - Size of each value
 *
 * @return true if key_compare is not NULL and key and value sizes are not zero
 */
bool init_tmmap                     (tree_multimap *tree,
                                     key_cmp *key_compare,
                                     size_t key_size,
                                     size_t value_size);

/**
 * Frees a tree multimap, making it the same as uninitialized.
 *
 * @param tree - Pointer to initialized tree multimap
 */
void free_tmmap                     (tree_multimap *tree);

/**
 * Clears a tree multimap by deallocating all nodes and setting size to zero.
 *
 * @param tree - Pointer to initialized tree multimap
 */
void tmmap_clear                    (tree_multimap *tree);

/**
 * Puts a key with corresponding value into the tree.
 *
 * @param tree - Pointer to initialized tree multimap
 * @param key - Pointer to key
 * @param value - Value: ignored if tree was initialized with 0 value size
 *
 * @returns true if operation succeeded
 */
bool tmmap_put                      (tree_multimap *restrict tree,
                                     const void *restrict key,
                                     const void *restrict value);

/**
 * Puts a key with corresponding value into the tree only if key does not
 * exist in the tree. If such key already exists, false is returned
 *
 * @param tree - Pointer to initialized tree multimap
 * @param key - Pointer to key
 * @param value - Value: ignored if tree was initialized with 0 value size
 *
 * @returns true if operation succeeded, false if key already exists or
 * memory could not be allocated
 */
bool tmmap_put_if_absent            (tree_multimap *restrict tree,
                                     const void *restrict key,
                                     const void *restrict value);

/**
 * Removes a key along with its values
 *
 * @param tree - Pointer to initialized tree multimap
 * @param key - Pointer to key
 *
 * @return true if such a key was found and removed
 */
bool tmmap_remove                   (tree_multimap *restrict tree,
                                     const void *restrict key);

/**
 * Removes a range of values associated with the key. If the upper bound of
 * the range is more than the amount of values associated, it only removes up
 * until the last associated value. If the range covers all or more than all
 * values, then the key is removed from the multimap.
 *
 * @param tree - Pointer to initialized tree multimap
 * @param key - Pointer to key
 * @param lo - Lower bound of the removal range
 * @param hi - Upper bound of the removal range
 *
 * @return the actual amount of values removed
 */
size_t tmmap_remove_values          (tree_multimap *restrict tree,
                                     const void *restrict key,
                                     size_t lo,
                                     size_t hi);

/**
 * Checks if specified key exists
 *
 * @param tree - Pointer to initialized tree multimap
 * @param key - Pointer to key
 *
 * @return true if such a key exists, false otherwise
 */
bool tmmap_has_key                  (const tree_multimap *restrict tree,
                                     const void *restrict key);

/**
 * Checks the number of keys that match of the specified key
 *
 * @param tree - Pointer to initialized tree multimap
 * @param key - Pointer to key
 *
 * @return the number of matching keys
 */
size_t tmmap_count_matches          (const tree_multimap *restrict tree,
                                     const void *restrict key);

/**
 * Returns the list of values corresponding to the specified key. If tree was
 * initialized with value size of 0, there is no value, and so will always
 * return NULL.
 *
 * @param tree - Pointer to initialized tree multimap
 * @param key - Pointer to key
 * @param out_matches - Pointer that will be filled with the number of
 *                      matches; ignored if NULL
 *
 * @return the list of values (as this is a multimap) or NULL
 */
const void *tmmap_get               (const tree_multimap *restrict tree,
                                     const void *restrict key,
                                     size_t *restrict out_matches);

/**
 * Returns the list of values corresponding to the specified key. If value
 * does not exist because it has not been inserted yet or if tree was
 * initialized with value size of 0, the default value will be returned.
 *
 * @param tree - Pointer to initialized tree multimap
 * @param key - Pointer to key
 * @param default_value - Default value
 *
 * @return the list of values (as this is a multimap) or the default value
 */
const void *tmmap_get_or_default    (const tree_multimap *restrict tree,
                                     const void *key,
                                     const void *default_value);

/**
 * Iterates through every key-value entry of the tree. The state of the tree
 * should be kept consistent during the iteration process.
 *
 * @param tree - Pointer to initialized tree multimap
 * @param it - An action to be performed on each entry
 */
void tmmap_foreach                  (const tree_multimap *tree,
                                     tmmap_it *it);

/**
 * Iterates through every key-value entry of the tree that is greater than the
 * specified key. The state of the tree should be kept consistent during the
 * iteration process.
 *
 * @param tree - Pointer to initialized tree multimap
 * @param it - An action to be performed on each entry
 * @param key - Key being compared
 */
void tmmap_foreach_gt               (const tree_multimap *restrict tree,
                                     const void *restrict key,
                                     tmmap_it *it);

/**
 * Iterates through every key-value entry of the tree that is lesser than the
 * specified key. The state of the tree should be kept consistent during the
 * iteration process.
 *
 * @param tree - Pointer to initialized tree multimap
 * @param it - An action to be performed on each entry
 * @param key - Key being compared
 */
void tmmap_foreach_lt               (const tree_multimap *restrict tree,
                                     const void *restrict key,
                                     tmmap_it *it);

/**
 * Returns the size of the tree multimap
 *
 * @param tree - Pointer to initialized tree multimap
 *
 * @return size of the tree multimap
 */
size_t tmmap_size                   (const tree_multimap *tree);

#endif