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

#ifndef __HASHMAP_H__
#define __HASHMAP_H__

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/**
 * The most primitive way of generating hashmaps
 *
 * @param id An unique name for this map
 * @param bucket_size Size of bucket. Either base of 2 or a prime number
 * @param K Data type of key
 * @param V Data type of value
 * @param hasher A function that takes the key and returns an integer hash
 * @param key_comparer A function that compares the keys
 * @param key_cloner A function that clones the key. To copy by value, ignore it.
 */
#define RAW_MAP_GENERATE(id, bucket_size, K, V, hasher, key_comparer, key_cloner) \
	struct __##id##_cell \
	{ \
		K key; V value; \
		struct __##id##_cell *next; \
	}; \
\
	struct id##_map_t \
	{ \
		size_t size; \
		struct __##id##_cell *b[bucket_size]; \
	}; \
\
	void \
	id##_for_each(struct id##_map_t *m, \
				  int (*v)(K, V)) \
	{ \
		size_t i; \
		for (i = 0; i < bucket_size; ++i) \
		{ \
			if (m->b[i] == NULL) continue; \
			struct __##id##_cell *s = m->b[i]; \
			do \
			{ \
				if (s->value == NULL) continue; \
				if (!v(s->key, s->value)) break; \
			} \
			while (s = s->next, s != NULL); \
		} \
	} \
 \
	V \
	id##_remove_value(struct id##_map_t *m, \
					  K k) \
	{ \
		const size_t d = hasher(k) % bucket_size; \
		if (m->b[d] == NULL) return NULL; \
		struct __##id##_cell *s = m->b[d]; \
		do \
		{ \
			if (key_comparer(s->key, k)) \
			{ \
				V p = s->value; \
				s->value = NULL; \
				m->size -= 1; \
				return p; \
			} \
		} \
		while (s = s->next, s != NULL); \
		return NULL; \
	} \
 \
	V \
	id##_get_value(struct id##_map_t *m, \
				   K k) \
	{ \
		const size_t d = hasher(k) % bucket_size; \
		if (m->b[d] == NULL) return NULL; \
		struct __##id##_cell *s = m->b[d]; \
		do \
		{ \
			if (key_comparer(s->key, k)) \
			{ \
				return s->value; \
			} \
		} \
		while (s = s->next, s != NULL); \
		return NULL; \
	} \
 \
	static inline \
	V \
	id##_get_or_default(struct id##_map_t *m, \
						K k, V d) \
	{ \
		V t = id##_get_value(m, k); \
		if (t == NULL) return d; \
		return t; \
	} \
 \
	V \
	id##_put_pair(struct id##_map_t *m, \
				  K k, V v) \
	{ \
		if (v == NULL) return NULL; \
		const size_t d = hasher(k) % bucket_size; \
		if (m->b[d] == NULL) \
		{ \
			m->b[d] = calloc(1, \
					sizeof (struct __##id##_cell)); \
			m->b[d]->key = key_cloner(k); \
			m->b[d]->value = v; \
			m->b[d]->next = NULL; \
			m->size += 1; \
			return NULL; \
		} \
		struct __##id##_cell *s = m->b[d]; \
		do \
		{ \
			if (key_comparer(s->key, k)) \
			{ \
				V p = s->value; \
				s->value = v; \
				return p; \
			} \
		} \
		while (s = s->next, s != NULL); \
 \
		s = calloc(1, \
				sizeof (struct __##id##_cell)); \
		s->key = key_cloner(k); \
		s->value = v; \
		s->next = NULL; \
		m->size += 1; \
		return NULL; \
	}

/**
 * A function that tests equality using the == operator
 */
#define basic_compare(a, b) ((a) == (b))

/**
 * Generates a hashmap with int as its key and a bucket_size of 256
 *
 * @param id A unique name for this map
 * @param V Data type of value
 * @param hasher A function that takes the key and returns an integer hash
 */
#define INT_MAP_GENERATE(id, V, hasher) \
		RAW_MAP_GENERATE(id, 256, int, V, hasher, basic_compare,)

/**
 * Generates a hashmap with char* as its key and a bucket_size of 256
 *
 * @param id An unique name for this map
 * @param V Data type of value
 * @param hasher A function that takes the key and returns an integer hash
 */
#define STR_MAP_GENERATE(id, V, hasher) \
		RAW_MAP_GENERATE(id, 256, char *, V, hasher, !strcmp, strdup)

/**
 * Returns the type of the map associated with the id
 *
 * Note: It really it just a macro expansion. You should always use this
 * instead of spelling out the types just in case the naming scheme got
 * changed (on purpose or by accident).
 *
 * @param id The unique name for the map
 *
 * @returns The expanded name
 */
#define MAP_TYPE(id) struct id##_map_t 

/**
 * Registers a value with the corresponding key. This will overwrite the
 * previous value when the same key is found.
 *
 * @param id The unique name for the map
 * @param map The map itself
 * @param key The key of the pair
 * @param value The value of the pair. If this is NULL, nothing happens
 *
 * @returns NULL for most cases. The previous value is returned if value
 * overwrites it.
 */
#define MAP_PUT_PAIR(id, map, key, value) id##_put_pair(map, key, value)

/**
 * Retrieves the value with the corresponding key
 *
 * @param id The unique name for the map
 * @param map The map itself
 * @param key The key of the pair
 *
 * @returns the value with the corresponding key. NULL if no key was found
 */
#define MAP_GET_VALUE(id, map, key) id##_get_value(map, key)

/**
 * Retrieves the value with the corresponding key if found
 *
 * @param id The unique name for the map
 * @param map The map itself
 * @param key The key of the pair
 * @param default_val Default return value
 *
 * @returns the value with the corresponding key. default_val is returned
 * if key does not exist
 */
#define MAP_GET_OR_DEFAULT(id, map, key, default_val) \
   id##_get_or_default(map, key, default_val)

/**
 * Checks if the key exists and has a value paired up with it
 *
 * @param id The unique name for the map
 * @param map The map itself
 * @param key The key of the pair
 *
 * @returns true if key exists. False otherwise
 */
#define MAP_HAS_KEY(id, map, key) (MAP_GET_VALUE(id, map, key) != NULL)

/**
 * Removes a key value pair from the map
 *
 * @param id The unique name for the map
 * @param map The map itself
 * @param key The key of the pair
 *
 * @returns the value being removed. NULL if key does not exist
 */
#define MAP_REMOVE_VALUE(id, map, key) id##_remove_value(map, key)

/**
 * Iterates through a map
 *
 * @param id The unique name for the map
 * @param map The map itself
 * @param visitor The action of each iteration. This function takes
 * a key and a value and returns a boolean. If false is returned
 * during the loop, it breaks and does not iterate through the
 * remaining elements.
 */
#define MAP_FOR_EACH(id, map, visitor) id##_for_each(map, visitor)

#endif
