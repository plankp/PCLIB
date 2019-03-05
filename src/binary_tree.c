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

#include "binary_tree.h"

#include <stdlib.h>
#include <string.h>

static inline
size_t calc_node_size
(binary_tree const * const tree, size_t n)
{
  return sizeof(tree_node) + tree->blk * n;
}

static
void free_subsequent_nodes
(tree_node * node)
{
  if (node == NULL) return;

  free_subsequent_nodes(node->lhs);
  free_subsequent_nodes(node->rhs);
  free(node);
}

static
void traversal_inorder
(tree_node const * node, bool const has_alloc_value, void (* it)(void const *, size_t, void const *))
{
  if (node == NULL) return;

  /* visit lhs, data then rhs */
  traversal_inorder(node->lhs, has_alloc_value, it);
  it(node->key, node->count, has_alloc_value ? node->values : NULL);
  traversal_inorder(node->rhs, has_alloc_value, it);
}

static
tree_node ** find_tree_node
(binary_tree const * restrict const tree, void const * restrict const key)
{
  /* this is more like a multimap meaning you can have
   * multiple nodes with the same key (compare --> 0)
   */
  tree_node * const * node = &tree->root;
  while (*node != NULL)
  {
    int const cmp = tree->key_compare((*node)->key, key);
    if (cmp > 0) node = &((*node)->rhs);
    else if (cmp < 0) node = &((*node)->lhs);
    else /* cmp == 0 */
    {
      break;
    }
  }
  return (tree_node **) node;
}

bool init_bintree
(binary_tree * const tree, key_cmp * key_compare, size_t data_size)
{
  if (key_compare == NULL) return false;

  tree->len = 0;
  tree->blk = data_size;
  tree->root = NULL;
  tree->key_compare = key_compare;
  return true;
}

void free_bintree
(binary_tree * const tree)
{
  bintree_clear(tree);
}

void bintree_clear
(binary_tree * const tree)
{
  if (tree->len > 0)
  {
    free_subsequent_nodes(tree->root);

    tree->len = 0;
    tree->root = NULL;
  }
}

bool bintree_put
(binary_tree * restrict const tree, void const * restrict key, void const * restrict value)
{
  tree_node ** node = find_tree_node(tree, key);
  if (*node == NULL)
  {
    /* create a blank node */
    tree_node *new_node = malloc(calc_node_size(tree, 1));
    if (new_node == NULL) return NULL;

    new_node->count = 1;
    new_node->key = key;
    new_node->lhs = NULL;
    new_node->rhs = NULL;
    memcpy(new_node->values, value, tree->blk);

    ++tree->len;
    *node = new_node;
    return true;
  }

  /* resize the current node */
  tree_node * current = *node;
  tree_node * resized = tree->blk == 0 ? current : realloc(current, calc_node_size(tree, current->count + 1));
  if (resized == NULL) return false;

  ++tree->len;
  memcpy(resized->values + tree->blk * resized->count++, value, tree->blk);
  *node = resized;
  return true;
}

bool bintree_put_if_absent
(binary_tree * restrict const tree, void const * restrict key, void const * restrict value)
{
  tree_node ** node = find_tree_node(tree, key);

  if (*node != NULL) return false;

  /* *node == NULL, which means creating a blank node */
  tree_node *new_node = malloc(calc_node_size(tree, 1));
  if (new_node == NULL) return NULL;

  new_node->count = 1;
  new_node->key = key;
  new_node->lhs = NULL;
  new_node->rhs = NULL;
  memcpy(new_node->values, value, tree->blk);

  ++tree->len;
  *node = new_node;
  return true;
}

bool bintree_remove
(binary_tree * restrict const tree, void const * restrict key)
{
  tree_node ** node = find_tree_node(tree, key);

  if (*node == NULL) return false;

  tree_node * current = *node;
  tree->len -= current->count;

  if (current->lhs == NULL)
  {
    /* pull rhs up to replace the current node */
    *node = current->rhs;
    /* make sure the pulled rhs does not get freed */
    current->rhs = NULL;
    free_subsequent_nodes(current);
    return true;
  }

  if (current->rhs == NULL)
  {
    /* pull lhs up to replace the current node */
    *node = current->lhs;
    /* make sure the pulled lhs does not get freed */
    current->lhs = NULL;
    free_subsequent_nodes(current);
    return true;
  }

  /* node contains both children: pull up the minimum node of rhs */
  tree_node *parent = current;
  tree_node *min = current->rhs;
  while (min->lhs != NULL)
  {
    parent = min;
    min = min->lhs;
  }

  if (parent == current)
  {
    /* parent           parent
      *  /  \             /  \
      * ??? min   -->   ???  min'
      *       \
      *       min'
      */
    parent->rhs = min->rhs;
  }
  else
  {
    /* parent           parent
      *  /  \             /  \
      * min ???   -->   min' ???
      *   \
      *   min'
      */
    parent->lhs = min->rhs;
  }

  /* swap out the current node (being removed) and fix links */
  *node = min;
  min->lhs = current->lhs;
  min->rhs = current->rhs;

  /* detach removed node's connections and clean it up */
  current->lhs = NULL;
  current->rhs = NULL;
  free_subsequent_nodes(current);

  return true;
}

bool bintree_has_key
(binary_tree const * restrict const tree, void const * restrict key)
{
  return bintree_count_matches(tree, key) > 0;
}

size_t bintree_count_matches
(binary_tree const * restrict const tree, void const * restrict key)
{
  tree_node ** node = find_tree_node(tree, key);
  return *node == NULL ? 0 : (*node)->count;
}

void const * bintree_get
(binary_tree const * restrict const tree, void const * restrict key, size_t * restrict matches)
{
  tree_node ** node = find_tree_node(tree, key);

  if (*node == NULL) return NULL;

  if (matches != NULL) *matches = (*node)->count;
  return tree->blk > 0 ? (*node)->values : NULL;
}

void const * bintree_get_or_default
(binary_tree const * restrict const tree, void const * key, void const * default_value)
{
  void const * value = bintree_get(tree, key, NULL);
  return value == NULL ? default_value : value;
}

void bintree_foreach
(binary_tree const * const tree, void (* it)(void const *, size_t, void const *))
{
  traversal_inorder(tree->root, tree->blk > 0, it);
}

size_t bintree_size
(binary_tree const * const tree)
{
  return tree->len;
}