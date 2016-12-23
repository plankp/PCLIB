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
		struct id##_bintree_t *p; \
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
			.p=NULL, \
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
	loop_state_t \
	id##_for_each (struct id##_bintree_t *t, \
				   loop_state_t (*v)(K, V)) \
	{ \
		if (t == NULL) return CONTINUE; \
		loop_state_t st = id##_for_each(t->l, v); \
		int is_last_cycle = 0; \
		if (st == ABORT) return st; \
		else if (st == ABORT_AFTER_CYCLE) is_last_cycle = 1; \
		st = v(t->k, t->v); \
		if (st == ABORT) return st; \
		else if (st == ABORT_AFTER_CYCLE) is_last_cycle = 1; \
		st = id##_for_each(t->r, v); \
		if (st == ABORT_AFTER_CYCLE || is_last_cycle) st = ABORT; \
		return st; \
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
		struct id##_bintree_t *p = NULL; \
		while ((*n) != NULL) \
		{ \
			int r = a->h - (*n)->h; \
			if (r == 0) \
			{ \
				r = key_comparer(a->k, (*n)->k); \
				if (r == 0) \
				{ \
					(*n)->p = a->p; \
					(*n)->l = a->l; \
					(*n)->r = a->r; \
					return; \
				} \
				goto cont_cycle; \
			} \
cont_cycle: \
			p = *n; \
			if (r < 0) n = &(*n)->l; \
			else n = &(*n)->r; \
		} \
		*n = a; \
		(*n)->p = p; \
	} \
 \
	struct id##_bintree_t * \
	id##_remove_key (struct id##_bintree_t *t, K k) \
	{ \
		struct id##_bintree_t *n = t; \
		const size_t pkh = hasher(k); \
		int ltp = 0; \
		while (n != NULL) \
		{ \
			int r = pkh - n->h; \
			if (r == 0) \
			{ \
				r = key_comparer(k, n->k); \
				if (r == 0) \
				{ \
					if (n->l == NULL) \
					{ \
						switch (ltp) \
						{ \
							case 1: n->p->l = n->r; break; \
							case 2: n->p->r = n->r; break; \
						} \
					} \
					else \
					{ \
						switch (ltp) \
						{ \
							case 1: n->p->l = n->l; break; \
							case 2: n->p->r = n->l; break; \
						} \
						if (n->r != NULL) \
						{ \
							struct id##_bintree_t *g = t; \
							while (g->p != NULL) g = g->p; \
							id##_insert_tree(g, n->r); \
						} \
					} \
					return n; \
				} \
				goto cont_cycle; \
			} \
cont_cycle: \
			if (r < 0) ltp = 1, n = n->l; \
			else ltp = 2, n = n->r; \
		} \
		return NULL; \
	}

#define INT_BINTREE_GENERATE(id, V, hasher) \
		RAW_BINTREE_GENERATE(id, int, V, hasher, basic_compare)

#define STR_BINTREE_GENERATE(id, V, hasher) \
		RAW_BINTREE_GENERATE(id, char *, V, hasher, strcmp)

#define BINTREE_TYPE(id) struct id##_bintree_t

#define BINTREE_INIT(id, key, value) id##_tree_init(key, value)

#define BINTREE_SIZE(id, tree) id##_tree_size(tree)

#define BINTREE_SEARCH(id, tree, key) id##_search_key(tree, key)

#define BINTREE_REMOVE(id, tree, key) id##_remove_key(tree, key)

#define BINTREE_INSERT(id, tree, new_tree) id##_insert_tree(tree, new_tree)

#define BINTREE_FOR_EACH(id, tree, visitor) id##_for_each(tree, visitor)

#endif
