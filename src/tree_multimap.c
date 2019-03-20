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

#include "tree_multimap.h"

#include <stdlib.h>
#include <string.h>

static inline
size_t calc_value_offset
(tree_multimap const * const tree, size_t n)
{
  return tree->key_blk + tree->value_blk * n;
}

static inline
size_t calc_node_size
(tree_multimap const * const tree, size_t n)
{
  return sizeof(tmmap_node) + calc_value_offset(tree, n);
}

static
tmmap_node *create_new_node
(tree_multimap const * restrict const tree, void const * restrict key, void const * restrict value)
{
  /* create a blank node */
  size_t const alloc_cap = TMMAP_GROW(1);
  tmmap_node *new_node = malloc(calc_node_size(tree, alloc_cap));
  if (new_node == NULL) return NULL;

  new_node->count = 1;
  new_node->cap = alloc_cap;
  new_node->lhs = NULL;
  new_node->rhs = NULL;

  /* copy the key */
  memcpy(new_node->data, key, tree->key_blk);
  /* copy the value */
  memcpy(new_node->data + calc_value_offset(tree, 0), value, tree->value_blk);

  return new_node;
}

static
void free_subsequent_nodes
(tmmap_node * node)
{
  if (node == NULL) return;

  free_subsequent_nodes(node->lhs);
  free_subsequent_nodes(node->rhs);
  free(node);
}

static
void traversal_inorder
(tree_multimap const * restrict const tree, tmmap_node const * node, tmmap_it * it)
{
  if (node == NULL) return;

  /* visit lhs, data then rhs */
  traversal_inorder(tree, node->lhs, it);
  it(node->data, node->count, node->data + calc_value_offset(tree, 0));
  traversal_inorder(tree, node->rhs, it);
}

static
void traversal_inorder_gt
(tree_multimap const * restrict const tree, tmmap_node const * node, void const * const restrict key, tmmap_it * it)
{
  if (node == NULL) return;

  /* visit lhs, data then rhs */
  traversal_inorder_gt(tree, node->lhs, key, it);
  if (tree->key_compare(key, node->data) < 0) it(node->data, node->count, node->data + calc_value_offset(tree, 0));
  traversal_inorder_gt(tree, node->rhs, key, it);
}

static
void traversal_inorder_lt
(tree_multimap const * restrict const tree, tmmap_node const * node, void const * const restrict key, tmmap_it * it)
{
  if (node == NULL) return;

  /* visit lhs, data then rhs */
  traversal_inorder_lt(tree, node->lhs, key, it);
  if (tree->key_compare(key, node->data) > 0) it(node->data, node->count, node->data + calc_value_offset(tree, 0));
  traversal_inorder_lt(tree, node->rhs, key, it);
}

static
tmmap_node ** find_tree_node
(tree_multimap const * restrict const tree, void const * restrict const key)
{
  /* this is more like a multimap meaning you can have
   * multiple nodes with the same key (compare --> 0)
   */
  tmmap_node * const * node = &tree->root;
  while (*node != NULL)
  {
    int const cmp = tree->key_compare(key, (*node)->data);
    if (cmp > 0) node = &(*node)->rhs;
    else if (cmp < 0) node = &(*node)->lhs;
    else /* cmp == 0 */
    {
      break;
    }
  }
  return (tmmap_node **) node;
}

bool init_tmmap
(tree_multimap * const tree, key_cmp * key_compare, size_t key_size, size_t value_size)
{
  if (key_compare == NULL || key_size == 0 || value_size == 0) return false;

  tree->len = 0;
  tree->key_blk = key_size;
  tree->value_blk = value_size;
  tree->root = NULL;
  tree->key_compare = key_compare;
  return true;
}

void free_tmmap
(tree_multimap * const tree)
{
  tmmap_clear(tree);
}

void tmmap_clear
(tree_multimap * const tree)
{
  if (tree->len > 0)
  {
    free_subsequent_nodes(tree->root);

    tree->len = 0;
    tree->root = NULL;
  }
}

bool tmmap_put
(tree_multimap * restrict const tree, void const * restrict key, void const * restrict value)
{
  tmmap_node ** node = find_tree_node(tree, key);
  if (*node == NULL)
  {
    tmmap_node *new_node = create_new_node(tree, key, value);
    if (new_node == NULL) return false;

    ++tree->len;
    *node = new_node;
    return true;
  }

  /* resize the current node */
  tmmap_node * current = *node;
  size_t const new_size = current->count + 1;
  if (current->cap < new_size)
  {
    size_t const new_cap = TMMAP_GROW(new_size);
    tmmap_node * resized = realloc(current, calc_node_size(tree, new_cap));
    if (resized == NULL) return false;
    resized->cap = new_cap;
    current = resized;
  }

  ++tree->len;
  memcpy(current->data + calc_value_offset(tree, current->count++), value, tree->value_blk);
  *node = current;
  return true;
}

bool tmmap_put_if_absent
(tree_multimap * restrict const tree, void const * restrict key, void const * restrict value)
{
  tmmap_node ** node = find_tree_node(tree, key);

  if (*node != NULL) return false;

  /* *node == NULL, which means creating a blank node */

  tmmap_node *new_node = create_new_node(tree, key, value);
  if (new_node == NULL) return false;

  ++tree->len;
  *node = new_node;
  return true;
}

static
void remove_node
(tmmap_node ** node)
{
  tmmap_node * current = *node;

  if (current->lhs == NULL)
  {
    /* pull rhs up to replace the current node */
    *node = current->rhs;
    /* make sure the pulled rhs does not get freed */
    current->rhs = NULL;
    free_subsequent_nodes(current);
    return;
  }

  if (current->rhs == NULL)
  {
    /* pull lhs up to replace the current node */
    *node = current->lhs;
    /* make sure the pulled lhs does not get freed */
    current->lhs = NULL;
    free_subsequent_nodes(current);
    return;
  }

  /* node contains both children: pull up the minimum node of rhs */
  tmmap_node *parent = current;
  tmmap_node *min = current->rhs;
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
}

bool tmmap_remove
(tree_multimap * restrict const tree, void const * restrict key)
{
  tmmap_node ** node = find_tree_node(tree, key);
  if (*node == NULL) return false;

  tree->len -= (*node)->count;
  remove_node(node);
  return true;
}

size_t tmmap_remove_values
(tree_multimap * restrict const tree, void const * restrict key, size_t lo, size_t hi)
{
  if (lo >= hi) return 0; /* do nothing since range is [lo, hi) */

  tmmap_node ** node = find_tree_node(tree, key);
  if (*node == NULL) return 0; /* actual amount of occurrences is 0 */

  tmmap_node * current = *node;
  /* only remove up to *count* if upper bound is out of range */
  size_t const max_len = current->count;
  size_t const max_idx = max_len < hi ? max_len : hi;

  /* if the lower bound is out of range, then this method does nothing */
  if (lo >= max_len) return 0;

  if (max_idx == max_len && lo == 0)
  {
    /* removing all values, just remove the node directly */
    tree->len -= max_len;
    remove_node(node);
    return max_len;
  }

  /* remove the values associated with the range */
  memmove(
    current->data + calc_value_offset(tree, lo),
    current->data + calc_value_offset(tree, max_idx),
    (max_len - max_idx) * tree->value_blk);
  size_t const delta = max_idx - lo;
  current->count -= delta;
  tree->len -= delta;
  return delta;
}

bool tmmap_has_key
(tree_multimap const * restrict const tree, void const * restrict key)
{
  return tmmap_count_matches(tree, key) > 0;
}

size_t tmmap_count_matches
(tree_multimap const * restrict const tree, void const * restrict key)
{
  tmmap_node ** node = find_tree_node(tree, key);
  return *node == NULL ? 0 : (*node)->count;
}

void const * tmmap_get
(tree_multimap const * restrict const tree, void const * restrict key, size_t * restrict matches)
{
  tmmap_node ** node = find_tree_node(tree, key);

  if (*node == NULL)
  {
    if (matches != NULL) *matches = 0;
    return NULL;
  }

  if (matches != NULL) *matches = (*node)->count;
  return (*node)->data + calc_value_offset(tree, 0);
}

void const * tmmap_get_or_default
(tree_multimap const * restrict const tree, void const * key, void const * default_value)
{
  void const * value = tmmap_get(tree, key, NULL);
  return value == NULL ? default_value : value;
}

void tmmap_foreach
(tree_multimap const * const tree, tmmap_it * it)
{
  traversal_inorder(tree, tree->root, it);
}

void tmmap_foreach_gt
(tree_multimap const * restrict const tree, void const * restrict key, tmmap_it * it)
{
  traversal_inorder_gt(tree, tree->root, key, it);
}

void tmmap_foreach_lt
(tree_multimap const * restrict const tree, void const * restrict key, tmmap_it * it)
{
  traversal_inorder_lt(tree, tree->root, key, it);
}

size_t tmmap_size
(tree_multimap const * const tree)
{
  return tree->len;
}