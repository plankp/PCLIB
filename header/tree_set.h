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

#ifndef __TREE_SET_H__
#define __TREE_SET_H__

#include <stddef.h>
#include <stdbool.h>

typedef int (key_cmp)(const void *, const void *);

typedef struct tset_node
{
  struct tset_node *lhs; /* less than */
  struct tset_node *rhs; /* more than */
  char data[];
} tset_node;

typedef struct tree_set
{
  size_t len;
  size_t key_blk;
  tset_node *root;
  key_cmp *key_compare;
} tree_set;

/**
 * Initializes a tree set with the specified key comparator.
 *
 * @param tree - Pointer to an uninitialized tree set
 * @param key_compare - Key relational comparator
 * @param key_size - Size of each key
 *
 * @return true if key_compare is not NULL and key_size is not zero
 */
bool init_tset                      (tree_set *tree,
                                     key_cmp *key_compare,
                                     size_t key_size);

/**
 * Frees a tree set, making it the same as uninitialized.
 *
 * @param tree - Pointer to initialized tree set
 */
void free_tset                      (tree_set *tree);

/**
 * Clears a tree set by deallocating all nodes and setting size to zero.
 *
 * @param tree - Pointer to initialized tree set
 */
void tset_clear                     (tree_set *tree);

/**
 * Puts a key into the tree.
 *
 * @param tree - Pointer to initialized tree set
 * @param key - Pointer to key
 *
 * @returns true if operation succeeded
 */
bool tset_put                       (tree_set *restrict tree,
                                     const void *restrict key);

/**
 * Puts a key into the tree only if key does not exist in the tree. If such
 * key already exists, false is returned
 *
 * @param tree - Pointer to initialized tree set
 * @param key - Pointer to key
 *
 * @returns true if operation succeeded, false if key already exists or
 * memory could not be allocated
 */
bool tset_put_if_absent             (tree_set *restrict tree,
                                     const void *restrict key);

/**
 * Removes a key
 *
 * @param tree - Pointer to initialized tree set
 * @param key - Pointer to key
 *
 * @return true if such a key was found and removed
 */
bool tset_remove                    (tree_set *restrict tree,
                                     const void *restrict key);

/**
 * Checks if specified key exists
 *
 * @param tree - Pointer to initialized tree set
 * @param key - Pointer to key
 *
 * @return true if such a key exists, false otherwise
 */
bool tset_has_key                   (const tree_set *restrict tree,
                                     const void *restrict key);

/**
 * Iterates through every key of the tree in sorted order. The state of the
 * tree should be kept consistent during the iteration process.
 *
 * @param tree - Pointer to initialized tree set
 * @param it - An action to be performed on each entry
 */
void tset_foreach                   (const tree_set *tree,
                                     void (*it)(const void *));

/**
 * Iterates through every key of the tree that is greater than the specified
 * key in sorted order. The state of the tree should be kept consistent during
 * the iteration process.
 *
 * @param tree - Pointer to initialized tree set
 * @param it - An action to be performed on each entry
 * @param key - Key being compared
 */
void tset_foreach_gt                (const tree_set *restrict tree,
                                     const void *restrict key,
                                     void (*it)(const void *));

/**
 * Iterates through every key of the tree that is lesser than the specified
 * key in sorted order. The state of the tree should be kept consistent during
 * the iteration process.
 *
 * @param tree - Pointer to initialized tree set
 * @param it - An action to be performed on each entry
 * @param key - Key being compared
 */
void tset_foreach_lt                (const tree_set *restrict tree,
                                     const void *restrict key,
                                     void (*it)(const void *));

/**
 * Returns the size of the tree set
 *
 * @param tree - Pointer to initialized tree set
 *
 * @return size of the tree set
 */
size_t tset_size                    (const tree_set *tree);

#endif