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

#ifndef __HASH_MAP_H__
#define __HASH_MAP_H__

#include <stddef.h>
#include <stdbool.h>

/**
 * The growth function for the hash map.
 *
 * By default, the capacity is scaled to the next multiple of 16.
 *
 * @param n - The precomputed new capacity
 *
 * @return value >= n
 */
#ifndef HMAP_GROW /* (size_t n) */
#define HMAP_GROW(n) ((((n) / 16) + 1) * 16)
#endif

/**
 * The probe function for locating buckets
 *
 * By default, linear probing is used (base hash + iteration)
 *
 * @param h - Base hash
 * @param k - Iteration
 *
 * @return potential bucket location
 */
#ifndef HMAP_PROBE /* (unsigned long h, size_t k) */
#define HMAP_PROBE(h, k) ((h) + (k))
#endif

typedef unsigned long (hash_func)(void const *);
typedef bool (key_eq)(void const *, void const *);

typedef struct map_entry
{
  unsigned long hash;
  void const * pair;
} map_entry;

typedef struct hash_map
{
  size_t len;
  size_t cap;
  map_entry * mem;
  hash_func * hasher;
  key_eq * key_equal;
} hash_map;

/**
 * Initializes a hash map with the specified hash function and key comparator.
 *
 * @param map - Pointer to an uninitialized hash map
 * @param hasher - Hash function
 * @param key_equal - Key equality comparator
 *
 * @return true if hasher and key_equal were not NULL
 */
bool init_hmap                    (hash_map * const map,
                                   hash_func * hasher,
                                   key_eq * key_equal);

/**
 * Frees a hash map, making it the same as uninitialized.
 *
 * @param map - Pointer to initialized hash map
 */
void free_hmap                    (hash_map * const map);

/**
 * Clears the hash map by marking the buckets as unoccupied. The size is set
 * to zero.
 *
 * @param map - Pointer to initialized hash map
 */
void hmap_clear                   (hash_map * const map);

/**
 * Ensures the capacity is at least n.
 *
 * @param map - Pointer to initialized hash map
 * @param n - New minimum capacity of the list
 *
 * @return true if capacity was already at least n or buckets were able to be
 * allocated successfully
 */
bool hmap_ensure_capacity         (hash_map * const map, size_t n);

/**
 * Puts a key-value pair into the map. If map already contains the same key,
 * the existant key-value pair will be saved to provided pointer and it will
 * be replaced by the new key-value pair.
 *
 * @param map - Pointer to initialized hash map
 * @param pair - Pointer to key-value pair
 * @param repl - Modified to old key-value pair if replacement took place; ignored if NULL
 *
 * @return true if pair was successfully placed in
 */
bool hmap_put                     (hash_map * restrict const map,
                                   void const * restrict pair,
                                   void const ** restrict repl);

/**
 * Puts a key-value pair into the map only if map does not contain a pair
 * with the same key.
 *
 * @param map - Pointer to initialized hash map
 * @param pair - Pointer to key-value pair
 *
 * @return true if pair was successfully placed in
 */
bool hmap_put_if_absent           (hash_map * restrict const map,
                                   void const * restrict pair);

/**
 * Replaces exisiting pair with the same key with a new key-value pair. Does
 * nothing if there are no exisiting pairs with the same key.
 *
 * @param map - Pointer to initialized hash map
 * @param pair - Pointer to key-value pair
 *
 * @return Pointer to exisiting pair or NULL if no such pair exists
 */
void const * hmap_replace         (hash_map * restrict const map,
                                   void const * restrict pair);

/**
 * Removes a pair with the same key
 *
 * @param map - Pointer to initialized hash map
 * @param pair - Pointer to key-value pair, only key is used
 *
 * @return Pointer to removed pair or NULL if no such pair exists
 */
void const * hmap_remove          (hash_map * restrict const map,
                                   void const * restrict pair);

/**
 * Checks if a pair with specified key exists
 *
 * @param map - Pointer to initialized hash map
 * @param pair - Pointer to key-value pair, only key is used
 *
 * @return true if such pair exists, false otherwise
 */
bool hmap_has_key                 (hash_map const * restrict const map,
                                   void const * restrict pair);

/**
 * Retrieves a pair with the specified key
 *
 * @param map - Pointer to initialized hash map
 * @param pair - Pointer to key-value pair, only key is used
 *
 * @return Pointer to such pair or NULL if no such pair exists
 */
void const * hmap_get             (hash_map const * restrict const map,
                                   void const * restrict pair);

/**
 * Retrieves a pair with the specified key or the default value if no such
 * pair exists
 *
 * @param map - Pointer to initialized hash map
 * @param pair - Pointer to key-value pair: this is also the default value
 *
 * @return Pointer to such pair or default value if no such pair exists
 */
void const * hmap_get_or_default  (hash_map const * restrict const map,
                                   void const * restrict pair);

/**
 * Forces the map to rehash all pairs with the new hash function. Internal
 * hash function will only be updated if rehash operation was successful.
 *
 * @param map - Pointer to initialized hash map
 * @param hasher - New hash function
 *
 * @return true if rehash was successful, false otherwise
 */
bool hmap_rehash                  (hash_map * const map, hash_func * hasher);

/**
 * Iterates through every pair of the map. The state of the map, apart from
 * the pairs stored in the map should be kept consistent during the iteration
 * process.
 *
 * @param map - Pointer to initialized hash map
 * @param it - An action to be performed on each pair
 */
void hmap_foreach                 (hash_map const * const map,
                                   void (* it)(void const *));

/**
 * Returns the size of the hash map
 *
 * @param map - Pointer to initialized hash map
 *
 * @return size of the hash map
 */
size_t hmap_size                  (hash_map const * const map);

/**
 * Returns the capacity of the hash map
 *
 * @param map - Pointer to initialized hash map
 *
 * @return capacity of the hash map
 */
size_t hmap_capacity              (hash_map const * const map);

#endif