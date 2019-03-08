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

#ifndef __BIT_ARRAY_H__
#define __BIT_ARRAY_H__

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct bit_array
{
  size_t c;
  uint32_t *b;
} bit_array;

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
bit_array init_bitarr (size_t bits);

/**
 * Destructs the bit array by freeing the underlying bit storage.
 *
 * @param arr The bit array being cleaned up
 */
void free_bitarr (bit_array *arr);

/**
 * Clears a bit array by unsetting all bits making the underlying
 * storage containing only false bits (0).
 *
 * @param arr The bit array being falsed
 */
void bitarr_clear (bit_array *arr);

/**
 * Toggles a specific bit of a bit array. If the bit was previously
 * false (0), it becomes true (1) and vice-versa. Nothing happens if
 * the specified bit is out of range.
 *
 * @param arr The bit array being toggled
 * @param bit The specified bit to be toggled
 */
void bitarr_toggle (bit_array *arr, size_t bit);

/**
 * Unsets a specific bit of a bit array. In other words, turing a
 * bit into false (0). Nothing happens if the specified bit is out
 * of range.
 *
 * @param arr The bit array being unsetted
 * @param bit The specified bit to be unsetted
 */
void bitarr_unset (bit_array *arr, size_t bit);

/**
 * Sets a specific bit of a bit array. In other words, turing a bit
 * into true (1). Nothing happens if the specified bit is out
 * of range.
 *
 * @param arr The bit array being setted
 * @param bit The specified bit to be setted
 */
void bitarr_set (bit_array *arr, size_t bit);

/**
 * Retrieves the toggle status of a specific bit in a bit array.
 *
 * @param arr The bit array being queried
 * @param bit The specified bit to be queried
 *
 * @returns 1 if bit is true, 0 if bit is false
 */
int bitarr_get (bit_array *arr, size_t bit);

/**
 * Returns the size of the bit array. This value does not mean the
 * amount of 32 bit integers used but the number of bits.
 *
 * @param arr The bit array being queried
 *
 * @returns The number of bits of a bit array
 */
size_t bitarr_size (bit_array *arr);

/**
 * Iterates through a bit array. All bits are iterated as opposed
 * only the true bits are being iterated.
 *
 * @param arr The bit array being iterated
 * @param vis The callback function, takes a bit (bool) as
 *            parameter, returns loop_state_t
 */
void bitarr_for_each (bit_array *arr, void (*vis)(bool));

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
bit_array bitarr_and (bit_array *lhs, bit_array *rhs);

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
bit_array bitarr_or (bit_array *lhs, bit_array *rhs);

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

bit_array bitarr_xor (bit_array *lhs, bit_array *rhs);

/**
 * Performs a bitwise not of an array and returns a new bit
 * array with the resulting bits. The original array is not
 * modified.
 *
 * @param base Bit array
 *
 * @returns A bit array with the opposite toggled bits of base
 */
bit_array bitarr_not (bit_array *base);

#ifdef __cplusplus
}
#endif

#endif
