/* 
 * Copyright (c) 2016 Paul Teng
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

#ifndef __VECTOR_H__
#define __VECTOR_H__

#include "utils.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define RAW_VECTOR_GENERATE(id, E, scale_factor) \
	struct id##_vector_t \
	{ \
		size_t c, l; E *b; \
	}; \
 \
	struct id##_vector_t \
	init_##id##_vector(size_t c) \
	{ \
		return (struct id##_vector_t) { \
			.c=c, .l=0, .b=calloc(c, sizeof(E)) \
		}; \
	} \
 \
	E * \
	id##_vector_get(struct id##_vector_t *v, size_t i) \
	{ \
		if (i < v->l) return v->b + i; \
		return NULL; \
	} \
 \
	void \
	id##_vector_for_each(struct id##_vector_t *v, \
						 loop_state_t (*vis)(E)) \
	{ \
		size_t i; \
		for (i = 0; i < v->l; ++i) \
			if (vis(v->b[i]) != CONTINUE) break; \
	} \
 \
	_Bool \
	id##_vector_ensure_cap (struct id##_vector_t *v, \
							size_t c) \
	{ \
		if (v->c < c) \
		{ \
			size_t nc = v->c * scale_factor; \
			while (nc < c) nc *= scale_factor; \
			E *np = realloc(v->b, nc * sizeof (E)); \
			return np ? v->b = np, v->c = nc, 1 : 0; \
		} \
		return 1; \
	} \
 \
	void \
	id##_vector_trim_unused(struct id##_vector_t *v) \
	{ \
		if (v->c != v->l) \
		{ \
			if (v->l == 0) \
			{ \
				free(v->b), v->b = NULL, v->c = 0; \
				return; \
			} \
			v->b = realloc(v->b, (v->c = v->l) * sizeof (E)); \
		} \
	} \
 \
	void \
	id##_vector_clear(struct id##_vector_t *v) \
	{ \
		v->l = 0; \
	} \
 \
	size_t \
	id##_vector_size(struct id##_vector_t *v) \
	{ \
		return v->l; \
	} \
 \
	void \
	clean_##id##_vector(struct id##_vector_t *v) \
	{ \
		v->l = 0, id##_vector_trim_unused(v); \
	} \
 \
	_Bool \
	id##_vector_push(struct id##_vector_t *v, E e) \
	{ \
		return id##_vector_ensure_cap(v, v->l + 1) ? v->b[v->l++] = e, 1 : 0; \
	} \
 \
	E * \
	id##_vector_pop(struct id##_vector_t *v, E *d) \
	{ \
		if (v->l > 0) \
		{ \
			v->l -= 1; \
			if (d != NULL) *d = v->b[v->l]; \
		} \
		else d = NULL; \
		return d; \
	} \
 \
	E * \
	id##_vector_remove(struct id##_vector_t *v, size_t i, E *d) \
	{ \
		if (i == v->l - 1) return id##_vector_pop(v, d); \
		if (i < v->l) \
		{ \
			if (d != NULL) *d = v->b[i]; \
			memmove(v->b + i, v->b + i + 1, (--v->l - i) * sizeof (E)); \
		} \
		else d = NULL; \
		return d; \
	} \
 \
	_Bool \
	id##_vector_insert(struct id##_vector_t *v, size_t i, E e) \
	{ \
		if (i == v->l - 1) return id##_vector_push(v, e); \
		if (i < v->l) \
		{ \
			if (id##_vector_ensure_cap(v, v->l + 1)) \
			{ \
				memmove(v->b + i + 1, v->b + i, (v->l++ - i) * sizeof (E)); \
				return v->b[i] = e, 1; \
			} \
		} \
		return 0; \
	}

#define VECTOR_GENERATE(id, E) RAW_VECTOR_GENERATE(id, E, 2)

#define INT_VEC_GENERATE(id) VECTOR_GENERATE(id, int)

#define STR_VEC_GENERATE(id) VECTOR_GENERATE(id, char *)

#define VECTOR_TYPE(id) struct id##_vector_t

#define VECTOR_INIT(id, capacity) (init_##id##_vector(capacity))

#define VECTOR_CLEAN(id, vector) (clean_##id##_vector(vector))

#define VECTOR_GET(id, vector, idx) (id##_vector_get(vector, idx))

#define VECTOR_FOR_EACH(id, vector, visitor) \
		(id##_vector_for_each(vector, visitor))

#define VECTOR_ENSURE_CAPACITY(id, vector, capacity) \
		(id##_vector_ensure_cap(vector, capacity))

#define VECTOR_TRIM_UNUSED(id, vector) \
		(id##_vector_trim_unused(vector))

#define VECTOR_CLEAR(id, vector) (id##_vector_clear(vector))

#define VECTOR_SIZE(id, vector) (id##_vector_size(vector))

#define VECTOR_PUSH(id, vector, item) (id##_vector_push(vector, item))

#define VECTOR_POP(id, vector, dest) (id##_vector_pop(vector, dest))

#define VECTOR_REMOVE(id, vector, idx, dest) \
		(id##_vector_remove(vector, idx, dest))

#define VECTOR_INSERT(id, vector, idx, item) \
		(id##_vector_insert(vector, idx, item))

#endif
