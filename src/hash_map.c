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
    unsigned long hashcode = hasher == NULL ? src[i].hash : hasher(pair);
    size_t offset = hashcode % dst_cap;

    map_entry *slot = &dst[offset];
    while (slot->pair != NULL)
    {
      /*
       * no need to check if two have same key since
       * map cannot have this (hmap_put handles this)
       */

      /* linear probing, +1 and try again */
      slot = &src[++offset % dst_cap];
    }

    /* place pair into slot */
    slot->hash = hashcode;
    slot->pair = pair;
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

  size_t const new_cap = ((n / 16) + 1) * 16;
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
  size_t offset = hashcode % cap;

  map_entry * slot = &map->mem[offset];
  while (slot->pair != NULL)
  {
    /* check if the slot has same key */
    if (slot->hash == hashcode && map->key_equal(slot->pair, pair))
    {
      /* if it does, replace the slot, and length is not changed */
      --map->len;
      if (repl != NULL)
      {
        *repl = slot->pair;
      }
      break;
    }

    /* linear probing, +1 and try again */
    slot = &map->mem[++offset % cap];
  }

  /* place pair into slot */
  slot->hash = hashcode;
  slot->pair = pair;
  ++map->len;
  return true;
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
  size_t offset = hashcode % cap;

  map_entry * slot = &map->mem[offset];
  while (slot->pair != NULL)
  {
    /* check if the slot has same key */
    if (slot->hash == hashcode && map->key_equal(slot->pair, pair))
    {
      /* if it does, do nothing since its put if absent */
      return true;
    }

    /* linear probing, +1 and try again */
    slot = &map->mem[++offset % cap];
  }

  /* place pair into slot */
  slot->hash = hashcode;
  slot->pair = pair;
  ++map->len;
  return true;
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
  size_t offset = hashcode % cap;

  map_entry * slot = &map->mem[offset];
  while (slot->pair != NULL)
  {
    /* check if the slot has same key */
    if (slot->hash == hashcode && map->key_equal(slot->pair, pair))
    {
      /* if it does, remove the slot */
      void * old = slot->pair;
      slot->pair = NULL;
      return old;
    }

    /* linear probing, +1 and try again */
    slot = &map->mem[++offset % cap];
  }

  /* nothing to remove */
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
  size_t offset = hashcode % cap;

  map_entry * slot = &map->mem[offset];
  while (slot->pair != NULL)
  {
    /* check if the slot has same key */
    if (slot->hash == hashcode && map->key_equal(slot->pair, pair))
    {
      /* if it does, replace the slot */
      void * old = slot->pair;
      slot->pair = pair;
      return old;
    }

    /* linear probing, +1 and try again */
    slot = &map->mem[++offset % cap];
  }

  /* nothing to replace with */
  return NULL;
}

bool hmap_has_key
(hash_map * restrict const map, void * restrict pair)
{
  if (pair == NULL || map->len < 1)
  {
    /* no such key */
    return false;
  }

  unsigned long hashcode = map->hasher(pair);
  size_t const cap = map->cap;
  size_t offset = hashcode % cap;

  map_entry slot = map->mem[offset];
  while (slot.pair != NULL)
  {
    /* check if the slot has same key */
    if (slot.hash == hashcode && map->key_equal(slot.pair, pair))
    {
      return true;
    }

    /* linear probing, +1 and try again */
    slot = map->mem[++offset % cap];
  }

  /* does not exist */
  return false;
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
  size_t offset = hashcode % cap;

  map_entry slot = map->mem[offset];
  while (slot.pair != NULL)
  {
    /* check if the slot has same key */
    if (slot.hash == hashcode && map->key_equal(slot.pair, pair))
    {
      return slot.pair;
    }

    /* linear probing, +1 and try again */
    slot = map->mem[++offset % cap];
  }

  /* does not exist */
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