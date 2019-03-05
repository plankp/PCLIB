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

#ifndef __BINARY_TREE_H__
#define __BINARY_TREE_H__

#include <stddef.h>
#include <stdbool.h>

typedef int (key_cmp)(void const *, void const *);

typedef struct tree_node
{
  size_t count;
  void const * key;
  struct tree_node * lhs; /* less than */
  struct tree_node * rhs; /* more than */
  char values[];
} tree_node;

typedef struct binary_tree
{
  size_t len;
  size_t blk;
  tree_node * root;
  key_cmp * key_compare;
} binary_tree;

/**
 * Initializes a binary tree with the specified key comparator and value size.
 * By changing the size of each value, this can be either act like a multiset
 * or a multimap.
 *
 * @param tree - Pointer to an uninitialized binary tree
 * @param key_compare - Key relational comparator
 * @param data_size - Size of each value, 0 will cause multiset-like behaviour
 *
 * @return true if key_compare were not NULL
 */
bool init_bintree                     (binary_tree * const tree,
                                       key_cmp * key_compare,
                                       size_t data_size);

/**
 * Frees a binary tree, making it the same as uninitialized.
 *
 * @param tree - Pointer to initialized binary tree
 */
void free_bintree                     (binary_tree * const tree);

/**
 * Clears a binary tree by deallocating all nodes and setting size to zero.
 *
 * @param tree - Pointer to initialized binary tree
 */
void bintree_clear                    (binary_tree * const tree);

/**
 * Puts a key with corresponding value into the tree.
 *
 * @param tree - Pointer to initialized binary tree
 * @param key - Key
 * @param value - Value: ignored if tree was initialized with 0 value size
 *
 * @returns true if operation succeeded
 */
bool bintree_put                      (binary_tree * restrict const tree,
                                       void const * restrict key,
                                       void const * restrict value);

/**
 * Puts a key with corresponding value into the tree only if key does not exist
 * in the tree. If such key already exists, false is returned
 *
 * @param tree - Pointer to initialized binary tree
 * @param key - Key
 * @param value - Value: ignored if tree was initialized with 0 value size
 *
 * @returns true if operation succeeded, false if key already exists or
 * memory could not be allocated
 */
bool bintree_put_if_absent            (binary_tree * restrict const tree,
                                       void const * restrict key,
                                       void const * restrict value);

/**
 * Removes a key along with its values
 *
 * @param tree - Pointer to initialized binary tree
 * @param pair - Key
 *
 * @return true if such a key was found and removed
 */
bool bintree_remove                   (binary_tree * restrict const tree,
                                       void const * restrict key);

/**
 * Checks if specified key exists
 *
 * @param tree - Pointer to initialized binary tree
 * @param pair - Key
 *
 * @return true if such a key exists, false otherwise
 */
bool bintree_has_key                  (binary_tree const * restrict const tree,
                                       void const * restrict key);

/**
 * Checks the number of keys that match of the specified key
 *
 * @param tree - Pointer to initialized binary tree
 * @param pair - Key
 *
 * @return the number of matching keys
 */
size_t bintree_count_matches          (binary_tree const * restrict const tree,
                                       void const * restrict key);

/**
 * Returns the list of values corresponding to the specified key. If tree was
 * initialized with value size of 0, there is no value, and so will always
 * return NULL.
 *
 * @param tree - Pointer to initialized binary tree
 * @param pair - Key
 * @param out_matches - Pointer that will be filled with the number of matches; ignored if NULL
 *
 * @return the list of values (as this is a multimap) or NULL
 */
void const * bintree_get              (binary_tree const * restrict const tree,
                                       void const * restrict key,
                                       size_t * restrict out_matches);

/**
 * Returns the list of values corresponding to the specified key. If value
 * does not exist because it has not been inserted yet or if tree was
 * initialized with value size of 0, the default value will be returned.
 *
 * @param tree - Pointer to initialized binary tree
 * @param pair - Key
 * @param default_value - Default value
 *
 * @return the list of values (as this is a multimap) or the default value
 */
void const * bintree_get_or_default   (binary_tree const * restrict const tree,
                                       void const * key,
                                       void const * default_value);

/**
 * Iterates through every key-value entry of the tree. The state of the tree,
 * should be kept consistent during the iteration process.
 *
 * @param tree - Pointer to initialized binary tree
 * @param it - An action to be performed on each entry
 * (void const * key, size_t matches, void const * values)
 */
void bintree_foreach                  (binary_tree const * const tree,
                                       void (* it)(void const *, size_t, void const *));

/**
 * Returns the size of the binary tree
 *
 * @param tree - Pointer to initialized binary tree
 *
 * @return size of the binary tree
 */
size_t bintree_size                   (binary_tree const * const tree);

#endif