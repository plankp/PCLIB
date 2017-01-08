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

#include "bitarray.h"

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

	const size_t cap = bits / 32 + (bits % 32 == 0 ? 0 : 1);
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
bitarray_for_each(bitarray_t *arr, loop_state_t (*vis)(int))
{
	if (arr->c == 0 || arr->b == NULL) return;
	size_t i;
	for (i = 0; i < arr->c; ++i)
	{
		if (vis(bitarray_get(arr, i)) != CONTINUE) break;
	}
}

bitarray_t
bitarray_and(bitarray_t *lhs, bitarray_t *rhs)
{
	const size_t new_size = lhs->c > rhs->c ? lhs->c : rhs->c;
	bitarray_t arr = init_bitarray(new_size);

	size_t i;
	for (i = 0; i < new_size; ++i)
	{
		const int lb = bitarray_get(lhs, i);
		const int rb = bitarray_get(rhs, i);
		if (lb & rb) bitarray_set(&arr, i);
	}

	return arr;
}

bitarray_t
bitarray_or(bitarray_t *lhs, bitarray_t *rhs)
{
	const size_t new_size = lhs->c > rhs->c ? lhs->c : rhs->c;
	bitarray_t arr = init_bitarray(new_size);

	size_t i;
	for (i = 0; i < new_size; ++i)
	{
		const int lb = bitarray_get(lhs, i);
		const int rb = bitarray_get(rhs, i);
		if (lb | rb) bitarray_set(&arr, i);
	}

	return arr;
}

bitarray_t
bitarray_xor(bitarray_t *lhs, bitarray_t *rhs)
{
	const size_t new_size = lhs->c > rhs->c ? lhs->c : rhs->c;
	bitarray_t arr = init_bitarray(new_size);

	size_t i;
	for (i = 0; i < new_size; ++i)
	{
		const int lb = bitarray_get(lhs, i);
		const int rb = bitarray_get(rhs, i);
		if (lb ^ rb) bitarray_set(&arr, i);
	}

	return arr;
}

bitarray_t
bitarray_not(bitarray_t *base)
{
	bitarray_t arr = init_bitarray(base->c);

	size_t i;
	for (i = 0; i < base->c; ++i)
	{
		const int b = bitarray_get(base, i);
		if (!b) bitarray_set(&arr, i);
	}

	return arr;
}
