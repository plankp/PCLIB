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

#ifndef __TREE_MAP_H__
#define __TREE_MAP_H__

#include <stddef.h>
#include <stdbool.h>

typedef void (tmap_it)(void const *, size_t, void const *);
typedef int (key_cmp)(void const *, void const *);

typedef struct tree_node
{
  size_t count;
  struct tree_node * lhs; /* less than */
  struct tree_node * rhs; /* more than */
  char data[];
} tree_node;

typedef struct tree_map
{
  size_t len;
  size_t key_blk;
  size_t value_blk;
  tree_node * root;
  key_cmp * key_compare;
} tree_map;

/**
 * Initializes a tree map with the specified key comparator and value size. By
 * changing the size of each value, this can be either act like a multiset
 * or a multimap.
 *
 * @param tree - Pointer to an uninitialized tree map
 * @param key_compare - Key relational comparator
 * @param key_size - Size of each key
 * @param value_size - Size of each value, 0 will cause multiset-like behaviour
 *
 * @return true if key_compare or key_size were not NULL
 */
bool init_tmap                        (tree_map * const tree,
                                       key_cmp * key_compare,
                                       size_t key_size,
                                       size_t value_size);

/**
 * Frees a tree map, making it the same as uninitialized.
 *
 * @param tree - Pointer to initialized tree map
 */
void free_tmap                        (tree_map * const tree);

/**
 * Clears a tree map by deallocating all nodes and setting size to zero.
 *
 * @param tree - Pointer to initialized tree map
 */
void tmap_clear                       (tree_map * const tree);

/**
 * Puts a key with corresponding value into the tree.
 *
 * @param tree - Pointer to initialized tree map
 * @param key - Pointer to key
 * @param value - Value: ignored if tree was initialized with 0 value size
 *
 * @returns true if operation succeeded
 */
bool tmap_put                         (tree_map * restrict const tree,
                                       void const * restrict key,
                                       void const * restrict value);

/**
 * Puts a key with corresponding value into the tree only if key does not
 * exist in the tree. If such key already exists, false is returned
 *
 * @param tree - Pointer to initialized tree map
 * @param key - Pointer to key
 * @param value - Value: ignored if tree was initialized with 0 value size
 *
 * @returns true if operation succeeded, false if key already exists or
 * memory could not be allocated
 */
bool tmap_put_if_absent               (tree_map * restrict const tree,
                                       void const * restrict key,
                                       void const * restrict value);

/**
 * Removes a key along with its values
 *
 * @param tree - Pointer to initialized tree map
 * @param key - Pointer to key
 *
 * @return true if such a key was found and removed
 */
bool tmap_remove                      (tree_map * restrict const tree,
                                       void const * restrict key);

/**
 * Checks if specified key exists
 *
 * @param tree - Pointer to initialized tree map
 * @param key - Pointer to key
 *
 * @return true if such a key exists, false otherwise
 */
bool tmap_has_key                     (tree_map const * restrict const tree,
                                       void const * restrict key);

/**
 * Checks the number of keys that match of the specified key
 *
 * @param tree - Pointer to initialized tree map
 * @param key - Pointer to key
 *
 * @return the number of matching keys
 */
size_t tmap_count_matches             (tree_map const * restrict const tree,
                                       void const * restrict key);

/**
 * Returns the list of values corresponding to the specified key. If tree was
 * initialized with value size of 0, there is no value, and so will always
 * return NULL.
 *
 * @param tree - Pointer to initialized tree map
 * @param key - Pointer to key
 * @param out_matches - Pointer that will be filled with the number of
 *                      matches; ignored if NULL
 *
 * @return the list of values (as this is a multimap) or NULL
 */
void const * tmap_get                 (tree_map const * restrict const tree,
                                       void const * restrict key,
                                       size_t * restrict out_matches);

/**
 * Returns the list of values corresponding to the specified key. If value
 * does not exist because it has not been inserted yet or if tree was
 * initialized with value size of 0, the default value will be returned.
 *
 * @param tree - Pointer to initialized tree map
 * @param key - Pointer to key
 * @param default_value - Default value
 *
 * @return the list of values (as this is a multimap) or the default value
 */
void const * tmap_get_or_default      (tree_map const * restrict const tree,
                                       void const * key,
                                       void const * default_value);

/**
 * Iterates through every key-value entry of the tree. The state of the tree
 * should be kept consistent during the iteration process.
 *
 * @param tree - Pointer to initialized tree map
 * @param it - An action to be performed on each entry
 */
void tmap_foreach                     (tree_map const * const tree,
                                       tmap_it * it);

/**
 * Iterates through every key-value entry of the tree that is greater than the
 * specified key. The state of the tree should be kept consistent during the
 * iteration process.
 *
 * @param tree - Pointer to initialized tree map
 * @param it - An action to be performed on each entry
 * @param key - Key being compared
 */
void tmap_foreach_gt                  (tree_map const * restrict const tree,
                                       void const * restrict key,
                                       tmap_it * it);

/**
 * Iterates through every key-value entry of the tree that is lesser than the
 * specified key. The state of the tree should be kept consistent during the
 * iteration process.
 *
 * @param tree - Pointer to initialized tree map
 * @param it - An action to be performed on each entry
 * @param key - Key being compared
 */
void tmap_foreach_lt                  (tree_map const * restrict const tree,
                                       void const * restrict key,
                                       tmap_it * it);

/**
 * Returns the size of the tree map
 *
 * @param tree - Pointer to initialized tree map
 *
 * @return size of the tree map
 */
size_t tmap_size                      (tree_map const * const tree);

#endif