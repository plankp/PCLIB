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

bool init_bintree     (binary_tree * const tree, key_cmp * key_compare, size_t data_size);

void free_bintree     (binary_tree * const tree);

void bintree_clear    (binary_tree * const tree);

bool bintree_put      (binary_tree * restrict const tree, void const * key, void const * value);

bool bintree_put_if_absent    (binary_tree * restrict const tree, void const * key, void const * value);

bool bintree_remove   (binary_tree * restrict const tree, void const * restrict key);

bool bintree_has_key  (binary_tree const * restrict const tree, void const * restrict key);

size_t bintree_count_matches  (binary_tree const * restrict const tree, void const * restrict key);

void const * bintree_get  (binary_tree const * restrict const tree, void const * restrict key, size_t * restrict matches);

void const * bintree_get_or_default (binary_tree const * restrict const tree, void const * key, void const * default_value);

void bintree_foreach  (binary_tree const * const tree,
                       void (* it)(void const *, size_t, void const *));

size_t bintree_size   (binary_tree const * const tree);

#endif