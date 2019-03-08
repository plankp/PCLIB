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
  unsigned int *b;
} bit_array;

/**
 * Initializes a fixed bit array with the specified amount of bits.
 *
 * @param arr - Pointer to an uninitialized bit array
 * @param bits The number of bits for the bit array
 *
 * @returns true if bit array was successfully initialized
 */
bool init_bitarr (bit_array *arr, size_t bits);

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
 * Attempts to release unused underlying storage slots.
 *
 * @param arr - The bit array being compacted
 */
void bitarr_compact (bit_array *arr);

/**
 * Resizes the bit array to only accomodate the fixed amount of bits.
 * Shrinking a bit array will only change the logical size. To shrink the
 * underlying buffer, call bitarr_compact immediately after.
 *
 * @param arr - The bit array being resized
 * @param new_bits - The new number of bits being accomdated
 *
 * @return true if resize operation was successful
 */
bool bitarr_resize (bit_array *arr, size_t new_bits);

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
 * @returns true if bit is set, false if bit is unset
 */
bool bitarr_get (bit_array const *arr, size_t bit);

/**
 * Returns the number of bits of the bit array.
 *
 * @param arr The bit array being queried
 *
 * @returns The number of bits of a bit array
 */
size_t bitarr_size (bit_array const *arr);

/**
 * Iterates through a bit array. All bits are iterated as opposed
 * only the true bits are being iterated.
 *
 * @param arr The bit array being iterated
 * @param vis The callback function, takes a bit (bool) as
 *            parameter, returns loop_state_t
 */
void bitarr_for_each (bit_array const *arr, void (*vis)(bool));

/**
 * Performs a bitwise and between two arrays. Only the lhs is modified.
 *
 * If rhs has a smaller size than lhs, the result is as if rhs was padded with
 * false (0) bits until the dimensions match.
 *
 * @param lhs Bit array 1, will contain the result after
 * @param rhs Bit array 2
 *
 * @returns A bit array with the results
 */
void bitarr_and (bit_array *lhs, bit_array const *rhs);

/**
 * Performs a bitwise or between two arrays. Only the lhs is modified
 *
 * @param lhs Bit array 1, will contain the result after
 * @param rhs Bit array 2
 *
 * @returns A bit array with the results
 */
void bitarr_or (bit_array *lhs, bit_array const *rhs);

/**
 * Performs a bitwise xor between two arrays. Only the lhs is modified
 *
 * @param lhs Bit array 1, will contain the result after
 * @param rhs Bit array 2
 */

void bitarr_xor (bit_array *lhs, bit_array const *rhs);

/**
 * Performs a bitwise not of an array. The original array is modified.
 *
 * @param base Bit array, will contain the result after
 */
void bitarr_not (bit_array *base);

#endif
