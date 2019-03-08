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

static inline
size_t
compute_word_len(size_t bits)
{
	return bits / 32 + (bits % 32 == 0 ? 0 : 1);
}

bitarray_t
init_bitarray(size_t bits)
{
	// Using uint32_t, type guaranteed to be 32 bits!
	if (bits == 0)
	{
		return (bitarray_t) {
			.c=0, .b=NULL
		};
	}

	const size_t cap = compute_word_len(bits);
	return (bitarray_t) {
		.c=bits, .b=calloc(cap, sizeof (uint32_t))
	};
}

void
clean_bitarray(bitarray_t *arr)
{
	arr->c = 0;
	if (arr->b != NULL)
	{
		free(arr->b);
		arr->b = NULL;
	}
}

void
bitarray_clear(bitarray_t *arr)
{
	if (arr->c == 0 || arr->b == NULL) return;
	size_t cap = arr->c / 32 + (arr->c % 32 == 0 ? 0 : 1);
	while (cap-- > 0) arr->b[cap] = 0;
}

void
bitarray_toggle(bitarray_t *arr, size_t bit)
{
	if (bit >= arr->c || arr->b == NULL) return;
	arr->b[bit / 32] ^= 1 << (bit % 32);
}

void
bitarray_unset(bitarray_t *arr, size_t bit)
{
	if (bit >= arr->c || arr->b == NULL) return;
	arr->b[bit / 32] &= ~(1 << (bit % 32));
}

void
bitarray_set(bitarray_t *arr, size_t bit)
{
	if (bit >= arr->c || arr->b == NULL) return;
	arr->b[bit / 32] |= 1 << (bit % 32);
}

int
bitarray_get(bitarray_t *arr, size_t bit)
{
	if (bit >= arr->c || arr->b == NULL) return 0;
	return (arr->b[bit / 32] >> (bit % 32)) & 1;
}

size_t
bitarray_size(bitarray_t *arr)
{
	return arr->c;
}

void
bitarray_for_each(bitarray_t *arr, void (*vis)(bool))
{
	if (arr->c == 0 || arr->b == NULL) return;
	size_t i;
	for (i = 0; i < arr->c; ++i)
	{
		vis(bitarray_get(arr, i));
	}
}

bitarray_t
bitarray_and(bitarray_t *lhs, bitarray_t *rhs)
{
	const size_t new_size = lhs->c > rhs->c ? lhs->c : rhs->c;
	bitarray_t arr = init_bitarray(new_size);
	const size_t minwordlen = compute_word_len(
			lhs->c < rhs->c ? lhs->c : rhs->c);

	size_t i;
	for (i = 0; i < minwordlen; ++i) arr.b[i] = lhs->b[i] & rhs->b[i];

	// The remaining bits will guaranteed to be false since both
	// true is required to become true

	return arr;
}

bitarray_t
bitarray_or(bitarray_t *lhs, bitarray_t *rhs)
{
	bitarray_t arr;
	size_t i, minwordlen;
	if (lhs->c > rhs->c)
	{
		arr = init_bitarray(lhs->c);
		minwordlen = compute_word_len(rhs->c);
		memmove(arr.b + minwordlen, lhs->b + minwordlen,
				(compute_word_len(lhs->c) - minwordlen) * sizeof (uint32_t));
	}
	else
	{
		arr = init_bitarray(rhs->c);
		minwordlen = compute_word_len(lhs->c);
		memmove(arr.b + minwordlen, lhs->b + minwordlen,
				(compute_word_len(rhs->c) - minwordlen) * sizeof (uint32_t));
	}
	for (i = 0; i < minwordlen; ++i) arr.b[i] = lhs->b[i] | rhs->b[i];
	return arr;
}

bitarray_t
bitarray_xor(bitarray_t *lhs, bitarray_t *rhs)
{
	bitarray_t arr;
	size_t i, minwordlen;
	if (lhs->c > rhs->c)
	{
		arr = init_bitarray(lhs->c);
		minwordlen = compute_word_len(rhs->c);
		memmove(arr.b + minwordlen, lhs->b + minwordlen,
				(compute_word_len(lhs->c) - minwordlen) * sizeof (uint32_t));
	}
	else
	{
		arr = init_bitarray(rhs->c);
		minwordlen = compute_word_len(lhs->c);
		memmove(arr.b + minwordlen, lhs->b + minwordlen,
				(compute_word_len(rhs->c) - minwordlen) * sizeof (uint32_t));
	}
	for (i = 0; i < minwordlen; ++i) arr.b[i] = lhs->b[i] ^ rhs->b[i];
	return arr;

}

bitarray_t
bitarray_not(bitarray_t *base)
{
	bitarray_t arr = init_bitarray(base->c);
	size_t i, wordlen = compute_word_len(base->c);
	for (i = 0; i < wordlen; ++i) arr.b[i] = ~base->b[i];
	return arr;
}
