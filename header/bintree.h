#ifndef __BINTREE_H__
#define __BINTREE_H__

#include "utils.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define RAW_BINTREE_GENERATE(id, K, V, hasher, key_comparer) \
	struct id##_bintree_t \
	{ \
		K k; size_t h; V v; \
		struct id##_bintree_t *l; \
		struct id##_bintree_t *r; \
	}; \
 \
	struct id##_bintree_t \
	id##_tree_init (K k, V v) \
	{ \
		return (struct id##_bintree_t) { \
			.k=k, \
			.h=hasher(k), \
			.v=v, \
			.l=NULL, \
			.r=NULL \
		}; \
	} \
 \
	size_t \
	id##_tree_size (struct id##_bintree_t *t) \
	{ \
		return 1 \
			+ (t->l == NULL ? 0 : id##_tree_size(t->l)) \
			+ (t->r == NULL ? 0 : id##_tree_size(t->r)); \
	} \
 \
	struct id##_bintree_t * \
	id##_search_key (struct id##_bintree_t *t, K k) \
	{ \
		struct id##_bintree_t *n = t; \
		const size_t pkh = hasher(k); \
		while (n != NULL) \
		{ \
			int r = pkh - n->h; \
			if (r == 0) \
			{ \
				r = key_comparer(k, n->k); \
				if (r == 0) return n; \
				goto cont_cycle; \
			} \
cont_cycle: \
			if (r < 0) n = n->l; \
			else n = n->r; \
		} \
		return NULL; \
	} \
 \
	void \
	id##_insert_tree (struct id##_bintree_t *t, \
					  struct id##_bintree_t *a) \
	{ \
		struct id##_bintree_t **n = &t; \
		while ((*n) != NULL) \
		{ \
			int r = a->h - (*n)->h; \
			if (r == 0) \
			{ \
				r = key_comparer(a->k, (*n)->k); \
				if (r == 0) \
				{ \
					(*n)->l = a->l; \
					(*n)->r = a->r; \
					return; \
				} \
				goto cont_cycle; \
			} \
cont_cycle: \
			if (r < 0) n = &(*n)->l; \
			else n = &(*n)->r; \
		} \
		*n = a; \
	}

#define INT_BINTREE_GENERATE(id, V, hasher) \
		RAW_BINTREE_GENERATE(id, int, V, hasher, basic_compare)

#define STR_BINTREE_GENERATE(id, V, hasher) \
		RAW_BINTREE_GENERATE(id, char *, V, hasher, strcmp)

#define BINTREE_TYPE(id) struct id##_bintree_t

#define BINTREE_INIT(id, key, value) id##_tree_init(key, value)

#define BINTREE_SIZE(id, tree) id##_tree_size(tree)

#define BINTREE_SEARCH(id, tree, key) id##_search_key(tree, key)

#define BINTREE_INSERT(id, tree, new_tree) id##_insert_tree(tree, new_tree)

#endif
