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

#include "hash_map.h"

#include <stdlib.h>
#include <string.h>

static
void rehash_move
(map_entry * restrict dst, map_entry * restrict src,
 size_t const dst_cap, size_t const src_cap,
 hash_func * const hasher)
{
  if (dst_cap < 1) return;

  for (size_t i = 0; i < dst_cap; ++i)
  {
    dst[i].pair = NULL;
  }

  for (size_t i = 0; i < src_cap; ++i)
  {
    void * pair = src[i].pair;
    unsigned long const hashcode = hasher == NULL ? src[i].hash : hasher(pair);

    for (size_t k = 0; ; ++k)
    {
      size_t const offset = HMAP_PROBE(hashcode, k);
      map_entry * slot = &dst[offset % dst_cap];

      if (slot->pair == NULL)
      {
        /* place pair into slot */
        slot->hash = hashcode;
        slot->pair = pair;
        break;
      }
    }
  }
}

bool init_hmap
(hash_map * const map, hash_func * hasher, key_eq * key_equal)
{
  if (hasher == NULL || key_equal == NULL)
  {
    return false;
  }

  map->len = 0;
  map->cap = 0;
  map->mem = NULL;
  map->hasher = hasher;
  map->key_equal = key_equal;
  return true;
}

void free_hmap
(hash_map * const map)
{
  if (map->cap > 0)
  {
    free(map->mem);

    map->len = 0;
    map->cap = 0;
    map->mem = NULL;
    map->hasher = NULL;
  }
}

void hmap_clear
(hash_map * const map)
{
  map->len = 0;
  for (size_t i = 0; i < map->cap; ++i)
  {
    map->mem[i].pair = NULL;
  }
}

bool hmap_ensure_capacity
(hash_map * const map, size_t n)
{
  size_t const old_cap = map->cap;
  if (old_cap >= n)
  {
    /* capacity is already at least n */
    return true;
  }

  size_t const new_cap = HMAP_GROW(n);
  map_entry * new_mem = malloc(new_cap * sizeof(map_entry));
  if (new_mem == NULL)
  {
    return false;
  }

  rehash_move(new_mem, map->mem, new_cap, map->cap, NULL);

  map->cap = new_cap;

  free(map->mem);
  map->mem = new_mem;

  return true;
}

bool hmap_put
(hash_map * restrict const map, void * restrict pair, void ** restrict repl)
{
  if (pair == NULL)
  {
    /* insert null pair does nothing */
    return true;
  }

  if (!hmap_ensure_capacity(map, map->len + 1))
  {
    return false;
  }

  unsigned long hashcode = map->hasher(pair);
  size_t const cap = map->cap;

  for (size_t k = 0; ; ++k)
  {
    size_t const offset = HMAP_PROBE(hashcode, k);
    map_entry * slot = &map->mem[offset % cap];

    if (slot->pair == NULL)
    {
      /* place pair into empty slot */
      slot->hash = hashcode;
      slot->pair = pair;
      ++map->len;
      return true;
    }

    if (slot->hash == hashcode && map->key_equal(slot->pair, pair))
    {
      /* overwrite slot */
      if (repl != NULL)
      {
        *repl = slot->pair;
      }
      slot->hash = hashcode;
      slot->pair = pair;
      return true;
    }
  }
  return false;
}

bool hmap_put_if_absent
(hash_map * restrict const map, void * restrict pair)
{
  if (pair == NULL)
  {
    /* insert null pair does nothing */
    return true;
  }

  if (!hmap_ensure_capacity(map, map->len + 1))
  {
    return false;
  }

  unsigned long hashcode = map->hasher(pair);
  size_t const cap = map->cap;

  for (size_t k = 0; ; ++k)
  {
    size_t const offset = HMAP_PROBE(hashcode, k);
    map_entry * slot = &map->mem[offset % cap];

    if (slot->pair == NULL)
    {
      /* place pair into empty slot */
      slot->hash = hashcode;
      slot->pair = pair;
      ++map->len;
      return true;
    }

    if (slot->hash == hashcode && map->key_equal(slot->pair, pair))
    {
      /* put if absent, so do nothing and return false */
      return false;
    }
  }
  return false;
}

void * hmap_remove
(hash_map * restrict const map, void * restrict pair)
{
  if (pair == NULL || map->len < 1)
  {
    /* nothing is being removed */
    return NULL;
  }

  unsigned long hashcode = map->hasher(pair);
  size_t const cap = map->cap;

  for (size_t k = 0; ; ++k)
  {
    size_t const offset = HMAP_PROBE(hashcode, k);
    map_entry * slot = &map->mem[offset % cap];

    if (slot->pair == NULL)
    {
      /* does not exist, nothing to remove */
      return NULL;
    }

    if (slot->hash == hashcode && map->key_equal(slot->pair, pair))
    {
      /* it exists, remove the slot */
      void * old = slot->pair;
      slot->pair = NULL;
      return old;
    }
  }
  return NULL;
}

void * hmap_replace
(hash_map * restrict const map, void * restrict pair)
{
  if (pair == NULL || map->len < 1)
  {
    /* nothing is being replaced */
    return NULL;
  }

  unsigned long hashcode = map->hasher(pair);
  size_t const cap = map->cap;

  for (size_t k = 0; ; ++k)
  {
    size_t const offset = HMAP_PROBE(hashcode, k);
    map_entry * slot = &map->mem[offset % cap];

    if (slot->pair == NULL)
    {
      /* does not exist, nothing to replace */
      return NULL;
    }

    if (slot->hash == hashcode && map->key_equal(slot->pair, pair))
    {
      /* it exists, replace the slot */
      void * old = slot->pair;
      slot->pair = pair;
      return old;
    }
  }
  return NULL;
}

bool hmap_has_key
(hash_map * restrict const map, void * restrict pair)
{
  return hmap_get(map, pair) != NULL;
}

void * hmap_get
(hash_map * restrict const map, void * restrict pair)
{
  if (pair == NULL || map->len < 1)
  {
    return NULL;
  }

  unsigned long hashcode = map->hasher(pair);
  size_t const cap = map->cap;

  for (size_t k = 0; ; ++k)
  {
    size_t const offset = HMAP_PROBE(hashcode, k);
    map_entry * slot = &map->mem[offset % cap];
    if (slot->pair == NULL)
    {
      /* does not exist */
      return NULL;
    }

    if (slot->hash == hashcode && map->key_equal(slot->pair, pair))
    {
      return slot->pair;
    }
  }
  return NULL;
}

void * hmap_get_or_default
(hash_map * restrict const map, void * restrict pair)
{
  void * ptr = hmap_get(map, pair);
  if (ptr == NULL)
  {
    return pair;
  }
  return ptr;
}

bool hmap_rehash
(hash_map * const map, hash_func * hasher)
{
  if (map->len > 0)
  {
    size_t const cap = map->cap;
    map_entry * new_mem = malloc(cap * sizeof(map_entry));
    if (new_mem == NULL)
    {
      return false;
    }

    rehash_move(new_mem, map->mem, cap, cap, hasher);

    free(map->mem);
    map->mem = new_mem;
  }

  map->hasher = hasher;
  return true;
}

void hmap_foreach
(hash_map * const map, void (* it)(void const *))
{
  for (size_t i = 0; i < map->cap; ++i)
  {
    map_entry ent = map->mem[i];
    if (ent.pair != NULL)
    {
      it(ent.pair);
    }
  }
}

size_t hmap_size
(hash_map * const map)
{
  return map->len;
}

size_t hmap_capacity
(hash_map * const map)
{
  return map->cap;
}