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

#include "tree_multiset.h"

#include <stdlib.h>
#include <string.h>

static
tmset_node *create_new_node
(tree_multiset const * restrict const tree, void const * restrict key)
{
  /* create a blank node */
  tmset_node *new_node = malloc(sizeof(tmset_node) + tree->key_blk);
  if (new_node == NULL) return NULL;

  new_node->count = 1;
  new_node->lhs = NULL;
  new_node->rhs = NULL;

  /* copy the key */
  memcpy(new_node->data, key, tree->key_blk);

  return new_node;
}

static
void free_subsequent_nodes
(tmset_node * node)
{
  if (node == NULL) return;

  free_subsequent_nodes(node->lhs);
  free_subsequent_nodes(node->rhs);
  free(node);
}

static
void traversal_inorder
(tree_multiset const * restrict const tree, tmset_node const * node, tmset_it * it)
{
  if (node == NULL) return;

  /* visit lhs, data then rhs */
  traversal_inorder(tree, node->lhs, it);
  it(node->data, node->count);
  traversal_inorder(tree, node->rhs, it);
}

static
void traversal_inorder_gt
(tree_multiset const * restrict const tree, tmset_node const * node, void const * const restrict key, tmset_it * it)
{
  if (node == NULL) return;

  /* visit lhs, data then rhs */
  traversal_inorder_gt(tree, node->lhs, key, it);
  if (tree->key_compare(key, node->data) < 0) it(node->data, node->count);
  traversal_inorder_gt(tree, node->rhs, key, it);
}

static
void traversal_inorder_lt
(tree_multiset const * restrict const tree, tmset_node const * node, void const * const restrict key, tmset_it * it)
{
  if (node == NULL) return;

  /* visit lhs, data then rhs */
  traversal_inorder_lt(tree, node->lhs, key, it);
  if (tree->key_compare(key, node->data) > 0) it(node->data, node->count);
  traversal_inorder_lt(tree, node->rhs, key, it);
}

static
tmset_node ** find_tree_node
(tree_multiset const * restrict const tree, void const * restrict const key)
{
  /* this is more like a multimap meaning you can have
   * multiple nodes with the same key (compare --> 0)
   */
  tmset_node * const * node = &tree->root;
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
  return (tmset_node **) node;
}

bool init_tmset
(tree_multiset * const tree, key_cmp * key_compare, size_t key_size)
{
  if (key_compare == NULL || key_size == 0) return false;

  tree->len = 0;
  tree->key_blk = key_size;
  tree->root = NULL;
  tree->key_compare = key_compare;
  return true;
}

void free_tmset
(tree_multiset * const tree)
{
  tmset_clear(tree);
}

void tmset_clear
(tree_multiset * const tree)
{
  if (tree->len > 0)
  {
    free_subsequent_nodes(tree->root);

    tree->len = 0;
    tree->root = NULL;
  }
}

bool tmset_put
(tree_multiset * restrict const tree, void const * restrict key)
{
  tmset_node ** node = find_tree_node(tree, key);
  if (*node != NULL)
  {
    /* add it to existing entry by increasing count */
    ++(*node)->count;
  }
  else
  {
    /* allocate a new node */
    tmset_node *new_node = create_new_node(tree, key);
    if (new_node == NULL) return false;
    *node = new_node;
  }

  /* increment tree's size */
  ++tree->len;
  return true;
}

bool tmset_put_if_absent
(tree_multiset * restrict const tree, void const * restrict key)
{
  tmset_node ** node = find_tree_node(tree, key);

  if (*node != NULL) return false;

  /* *node == NULL, which means creating a blank node */
  tmset_node *new_node = create_new_node(tree, key);
  if (new_node == NULL) return false;

  ++tree->len;
  *node = new_node;
  return true;
}

static
void remove_node
(tmset_node ** node)
{
  tmset_node * current = *node;

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
  tmset_node *parent = current;
  tmset_node *min = current->rhs;
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

bool tmset_remove
(tree_multiset * restrict const tree, void const * restrict key)
{
  tmset_node ** node = find_tree_node(tree, key);
  if (*node == NULL) return false;

  tree->len -= (*node)->count;
  remove_node(node);
  return true;
}

size_t tmset_remove_count
(tree_multiset * restrict const tree, void const * restrict key, size_t const count)
{
  tmset_node ** node = find_tree_node(tree, key);
  if (*node == NULL) return 0; /* actual amount of occurrences is 0 */

  size_t const actual_occ = (*node)->count;
  if (actual_occ > count)
  {
    /* remove all *count* occurrences */
    (*node)->count -= count;
    tree->len -= count;
    return count;
  }

  /* remove the node */
  tree->len -= actual_occ;
  remove_node(node);
  return actual_occ;
}

bool tmset_has_key
(tree_multiset const * restrict const tree, void const * restrict key)
{
  return tmset_count_matches(tree, key) > 0;
}

size_t tmset_count_matches
(tree_multiset const * restrict const tree, void const * restrict key)
{
  tmset_node ** node = find_tree_node(tree, key);
  return *node == NULL ? 0 : (*node)->count;
}

void tmset_foreach
(tree_multiset const * const tree, tmset_it * it)
{
  traversal_inorder(tree, tree->root, it);
}

void tmset_foreach_gt
(tree_multiset const * restrict const tree, void const * restrict key, tmset_it * it)
{
  traversal_inorder_gt(tree, tree->root, key, it);
}

void tmset_foreach_lt
(tree_multiset const * restrict const tree, void const * restrict key, tmset_it * it)
{
  traversal_inorder_lt(tree, tree->root, key, it);
}

size_t tmset_size
(tree_multiset const * const tree)
{
  return tree->len;
}