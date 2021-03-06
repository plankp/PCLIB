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

#define WORDSZ    (sizeof(unsigned int))
#define WORDBITS  (WORDSZ * CHAR_BIT)

static inline
size_t
compute_word_len(size_t bits)
{
  return ((bits + WORDBITS - 1) / WORDBITS);
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
  unsigned int * buf = calloc(cap, WORDSZ);
  if (buf == NULL) return false;
  arr->c = bits;
  arr->b = buf;
  return true;
}

bool init_cpy_bitarr
(bit_array *arr, bit_array const *original)
{
  if (original == NULL) return false;
  if (!init_bitarr(arr, original->c)) return false;
  bitarr_or(arr, original);
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

void bitarr_compact
(bit_array *arr)
{
  size_t const len = compute_word_len(arr->c);
  unsigned int * new_buf = realloc(arr->b, len * WORDSZ);
  if (new_buf != NULL)
  {
    arr->b = new_buf;
  }

  /* if realloc failed (== NULL), continue:
   * standard says original memory is unchanged
   */
}

bool bitarr_resize
(bit_array *arr, size_t new_bits)
{
  /* case where array already accomodates those bits */
  if (arr->c == new_bits) return true;

  size_t const new_len = compute_word_len(new_bits);
  size_t const old_len = compute_word_len(arr->c);
  if (arr->c > new_bits)
  {
    /* shrink operation: do not resize underlying buffer,
     * just clear out the bits
     */
    arr->c = new_bits;
    for (size_t i = new_len; i < old_len; ++i)
    {
      arr->b[i] = 0;
    }
    return true;
  }

  /* grow the underlying buffer */
  unsigned int * new_buf = realloc(arr->b, new_len * WORDSZ);
  if (new_buf == NULL) return false;

  arr->c = new_bits;
  arr->b = new_buf;
  for (size_t i = old_len; i < new_len; ++i)
  {
    arr->b[i] = 0;
  }
  return true;
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
  arr->b[bit / WORDBITS] ^= 1 << (bit % WORDBITS);
}

void
bitarr_unset(bit_array *arr, size_t bit)
{
  if (bit >= arr->c || arr->b == NULL) return;
  arr->b[bit / WORDBITS] &= ~(1 << (bit % WORDBITS));
}

void
bitarr_set(bit_array *arr, size_t bit)
{
  if (bit >= arr->c || arr->b == NULL) return;
  arr->b[bit / WORDBITS] |= 1 << (bit % WORDBITS);
}

bool
bitarr_get(bit_array const *arr, size_t bit)
{
  if (bit >= arr->c || arr->b == NULL) return false;
  return (arr->b[bit / WORDBITS] >> (bit % WORDBITS)) & 1;
}

size_t
bitarr_size(bit_array const *arr)
{
  return arr->c;
}

void
bitarr_foreach(bit_array const *arr, void (*vis)(bool))
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

  /* need to account for boundary size difference:
   * (say WORDBITS is 8)        7 6 5 4 3 2 1 0
   * bit_array a with cap = 3: [0 0 0 0 0 0 0 0]
   * not a                   : [1 1 1 1 1 1 1 1]
   * need to correct bits [4, 7] since they are out of capacity:
   * correction mask         : [0 0 0 0 1 1 1 1]
   */
  if (wordlen > 0)
  {
    base->b[wordlen - 1] &= UINT_MAX >> (wordlen * WORDBITS - base->c);
  }
}
