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

#ifndef __BITARRAY_H__
#define __BITARRAY_H__

#include "utils.h"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct bitarray_t
{
	size_t c; uint32_t *b;
} bitarray_t;

#ifdef __cplusplus
extern "C"
{
#endif

bitarray_t init_bitarray (size_t bits);

void clean_bitarray (bitarray_t *arr);

void bitarray_clear (bitarray_t *arr);

void bitarray_toggle (bitarray_t *arr, size_t bit);

void bitarray_unset (bitarray_t *arr, size_t bit);

void bitarray_set (bitarray_t *arr, size_t bit);

int bitarray_get (bitarray_t *arr, size_t bit);

size_t bitarray_size (bitarray_t *arr);

void bitarray_for_each (bitarray_t *arr, loop_state_t (*vis)(int));

#ifdef __cplusplus
}
#endif

#endif
