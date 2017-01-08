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

/**
 * Initializes a fixed bit array with the specified amount of bits. The
 * underlying structure is implemented using 32 bit integers meaning
 * that the smallest amount of bits allocated is 32. This, however, does
 * not mean you can use all of the 32 bits if the amount of bits is less
 * than 32.
 *
 * @param bits The number of bits for the bit array
 * @returns The newly created bit array
 */
bitarray_t init_bitarray (size_t bits);

/**
 * Destructs the bit array by freeing the underlying bit storage.
 *
 * @param arr The bit array being cleaned up
 */
void clean_bitarray (bitarray_t *arr);

/**
 * Clears a bit array by unsetting all bits making the underlying
 * storage containing only false bits (0).
 *
 * @param arr The bit array being falsed
 */
void bitarray_clear (bitarray_t *arr);

/**
 * Toggles a specific bit of a bit array. If the bit was previously
 * false (0), it becomes true (1) and vice-versa. Nothing happens if
 * the specified bit is out of range.
 *
 * @param arr The bit array being toggled
 * @param bit The specified bit to be toggled
 */
void bitarray_toggle (bitarray_t *arr, size_t bit);

/**
 * Unsets a specific bit of a bit array. In other words, turing a
 * bit into false (0). Nothing happens if the specified bit is out
 * of range.
 *
 * @param arr The bit array being unsetted
 * @param bit The specified bit to be unsetted
 */
void bitarray_unset (bitarray_t *arr, size_t bit);

/**
 * Sets a specific bit of a bit array. In other words, turing a bit
 * into true (1). Nothing happens if the specified bit is out
 * of range.
 *
 * @param arr The bit array being setted
 * @param bit The specified bit to be setted
 */
void bitarray_set (bitarray_t *arr, size_t bit);

/**
 * Retrieves the toggle status of a specific bit in a bit array.
 *
 * @param arr The bit array being queried
 * @param bit The specified bit to be queried
 *
 * @returns 1 if bit is true, 0 if bit is false
 */
int bitarray_get (bitarray_t *arr, size_t bit);

/**
 * Returns the size of the bit array. This value does not mean the
 * amount of 32 bit integers used but the number of bits.
 *
 * @param arr The bit array being queried
 *
 * @returns The number of bits of a bit array
 */
size_t bitarray_size (bitarray_t *arr);

/**
 * Iterates through a bit array. All bits are iterated as opposed
 * only the true bits are being iterated.
 *
 * @param arr The bit array being iterated
 * @param vis The callback function, takes a bit (int) as
 *            parameter, returns loop_state_t
 */
void bitarray_for_each (bitarray_t *arr, loop_state_t (*vis)(int));

/**
 * Performs a bitwise and between two arrays and returns a new
 * bit array with the resulting bits. The two arrays are not
 * modified.
 *
 * @param lhs Bit array 1
 * @param rhs Bit array 2
 *
 * @returns A bit array with the results
 */
bitarray_t bitarray_and (bitarray_t *lhs, bitarray_t *rhs);

/**
 * Performs a bitwise or between two arrays and returns a new
 * bit array with the resulting bits. The two arrays are not
 * modified.
 *
 * @param lhs Bit array 1
 * @param rhs Bit array 2
 *
 * @returns A bit array with the results
 */
bitarray_t bitarray_or (bitarray_t *lhs, bitarray_t *rhs);

/**
 * Performs a bitwise xor between two arrays and returns a new
 * bit array with the resulting bits. The two arrays are not
 * modified.
 *
 * @param lhs Bit array 1
 * @param rhs Bit array 2
 *
 * @returns A bit array with the results
 */

bitarray_t bitarray_xor (bitarray_t *lhs, bitarray_t *rhs);

/**
 * Performs a bitwise not of an array and returns a new bit
 * array with the resulting bits. The original array is not
 * modified.
 *
 * @param base Bit array
 *
 * @returns A bit array with the opposite toggled bits of base
 */
bitarray_t bitarray_not (bitarray_t *base);

#ifdef __cplusplus
}
#endif

#endif
