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

#include "tree_set.h"

#include <stdlib.h>
#include <string.h>

typedef void tset_it(void const *);

static
tset_node *create_new_node
(tree_set const * restrict const tree, void const * restrict key)
{
  /* create a blank node */
  tset_node *new_node = malloc(sizeof(tset_node) + tree->key_blk);
  if (new_node == NULL) return NULL;

  new_node->lhs = NULL;
  new_node->rhs = NULL;

  /* copy the key */
  memcpy(new_node->data, key, tree->key_blk);

  return new_node;
}

static
void free_subsequent_nodes
(tset_node * node)
{
  if (node == NULL) return;

  free_subsequent_nodes(node->lhs);
  free_subsequent_nodes(node->rhs);
  free(node);
}

static
void traversal_inorder
(tree_set const * restrict const tree, tset_node const * node, tset_it * it)
{
  if (node == NULL) return;

  /* visit lhs, data then rhs */
  traversal_inorder(tree, node->lhs, it);
  it(node->data);
  traversal_inorder(tree, node->rhs, it);
}

static
void traversal_inorder_gt
(tree_set const * restrict const tree, tset_node const * node, void const * const restrict key, tset_it * it)
{
  if (node == NULL) return;

  /* visit lhs, data then rhs */
  traversal_inorder_gt(tree, node->lhs, key, it);
  if (tree->key_compare(key, node->data) < 0) it(node->data);
  traversal_inorder_gt(tree, node->rhs, key, it);
}

static
void traversal_inorder_lt
(tree_set const * restrict const tree, tset_node const * node, void const * const restrict key, tset_it * it)
{
  if (node == NULL) return;

  /* visit lhs, data then rhs */
  traversal_inorder_lt(tree, node->lhs, key, it);
  if (tree->key_compare(key, node->data) > 0) it(node->data);
  traversal_inorder_lt(tree, node->rhs, key, it);
}

static
tset_node ** find_tree_node
(tree_set const * restrict const tree, void const * restrict const key)
{
  tset_node * const * node = &tree->root;
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
  return (tset_node **) node;
}

bool init_tset
(tree_set * const tree, key_cmp * key_compare, size_t key_size)
{
  if (key_compare == NULL || key_size == 0) return false;

  tree->len = 0;
  tree->key_blk = key_size;
  tree->root = NULL;
  tree->key_compare = key_compare;
  return true;
}

void free_tset
(tree_set * const tree)
{
  tset_clear(tree);
}

void tset_clear
(tree_set * const tree)
{
  if (tree->len > 0)
  {
    free_subsequent_nodes(tree->root);

    tree->len = 0;
    tree->root = NULL;
  }
}

bool tset_put
(tree_set * restrict const tree, void const * restrict key)
{
  tset_node ** node = find_tree_node(tree, key);
  if (*node != NULL)
  {
    /* key already exists, which counts as successfully inserted
     * (difference from put_if_absent)
     */
    return true;
  }

  /* *node == NULL, which means creating a blank node */
  tset_node *new_node = create_new_node(tree, key);
  if (new_node == NULL) return false;

  ++tree->len;
  *node = new_node;
  return true;
}

bool tset_put_if_absent
(tree_set * restrict const tree, void const * restrict key)
{
  tset_node ** node = find_tree_node(tree, key);

  if (*node != NULL) return false;

  /* *node == NULL, which means creating a blank node */
  tset_node *new_node = create_new_node(tree, key);
  if (new_node == NULL) return false;

  ++tree->len;
  *node = new_node;
  return true;
}

bool tset_remove
(tree_set * restrict const tree, void const * restrict key)
{
  tset_node ** node = find_tree_node(tree, key);

  if (*node == NULL) return false;

  tset_node * current = *node;
  --tree->len;

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
  tset_node *parent = current;
  tset_node *min = current->rhs;
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

bool tset_has_key
(tree_set const * restrict const tree, void const * restrict key)
{
  return *find_tree_node(tree, key) != NULL;
}

void tset_foreach
(tree_set const * const tree, tset_it * it)
{
  traversal_inorder(tree, tree->root, it);
}

void tset_foreach_gt
(tree_set const * restrict const tree, void const * restrict key, tset_it * it)
{
  traversal_inorder_gt(tree, tree->root, key, it);
}

void tset_foreach_lt
(tree_set const * restrict const tree, void const * restrict key, tset_it * it)
{
  traversal_inorder_lt(tree, tree->root, key, it);
}

size_t tset_size
(tree_set const * const tree)
{
  return tree->len;
}