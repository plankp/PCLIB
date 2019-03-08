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

#include "bit_array.h"

#define WORDSZ  (sizeof(unsigned int))

static inline
size_t
compute_word_len(size_t bits)
{
  return ((bits + WORDSZ - 1) / WORDSZ) * WORDSZ;
}

bool
init_bitarr(bit_array *arr, size_t bits)
{
  if (bits == 0)
  {
    arr->c = 0;
    arr->b = NULL;
    return true;
  }

  const size_t cap = compute_word_len(bits);
  unsigned int * buf = calloc(cap, sizeof (unsigned int));
  if (buf == NULL) return false;
  arr->c = bits;
  arr->b = buf;
  return true;
}

void
free_bitarr(bit_array *arr)
{
  arr->c = 0;
  if (arr->b != NULL)
  {
    free(arr->b);
    arr->b = NULL;
  }
}

void
bitarr_clear(bit_array *arr)
{
  if (arr->c == 0 || arr->b == NULL) return;
  size_t cap = compute_word_len(arr->c);
  while (cap-- > 0) arr->b[cap] = 0;
}

void
bitarr_toggle(bit_array *arr, size_t bit)
{
  if (bit >= arr->c || arr->b == NULL) return;
  arr->b[bit / WORDSZ] ^= 1 << (bit % WORDSZ);
}

void
bitarr_unset(bit_array *arr, size_t bit)
{
  if (bit >= arr->c || arr->b == NULL) return;
  arr->b[bit / WORDSZ] &= ~(1 << (bit % WORDSZ));
}

void
bitarr_set(bit_array *arr, size_t bit)
{
  if (bit >= arr->c || arr->b == NULL) return;
  arr->b[bit / WORDSZ] |= 1 << (bit % WORDSZ);
}

bool
bitarr_get(bit_array const *arr, size_t bit)
{
  if (bit >= arr->c || arr->b == NULL) return 0;
  return arr->b[bit / WORDSZ] >> (bit % WORDSZ);
}

size_t
bitarr_size(bit_array const *arr)
{
  return arr->c;
}

void
bitarr_for_each(bit_array const *arr, void (*vis)(bool))
{
  if (arr->c == 0 || arr->b == NULL) return;
  size_t i;
  for (i = 0; i < arr->c; ++i)
  {
    vis(bitarr_get(arr, i));
  }
}

void
bitarr_and(bit_array *lhs, bit_array const *rhs)
{
  size_t const lhs_len = compute_word_len(lhs->c);
  size_t const rhs_len = compute_word_len(rhs->c);
  size_t const minwordlen = lhs_len > rhs_len ? rhs_len : lhs_len;
  for (size_t i = 0; i < minwordlen; ++i)
  {
    lhs->b[i] &= rhs->b[i];
  }

  /* set the remaining bits to false */
  for (size_t i = minwordlen; i < lhs_len; ++i)
  {
    lhs->b[i] = 0;
  }
}

void
bitarr_or(bit_array *lhs, bit_array const *rhs)
{
  size_t const minwordlen = compute_word_len(lhs->c > rhs->c ? rhs->c : lhs->c);
  for (size_t i = 0; i < minwordlen; ++i)
  {
    lhs->b[i] |= rhs->b[i];
  }
}

void
bitarr_xor(bit_array *lhs, bit_array const *rhs)
{
  size_t const minwordlen = compute_word_len(lhs->c > rhs->c ? rhs->c : lhs->c);
  for (size_t i = 0; i < minwordlen; ++i)
  {
    lhs->b[i] ^= rhs->b[i];
  }
}

void bitarr_not
(bit_array *base)
{
  size_t const wordlen = compute_word_len(base->c);
  for (size_t i = 0; i < wordlen; ++i)
  {
    base->b[i] = ~base->b[i];
  }
}
