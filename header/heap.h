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

#ifndef __HEAP_H__
#define __HEAP_H__

#include <stddef.h>

/**
 * Converts array to binary heap where the root node (first element) is
 * greater or equal to all its children.
 *
 * @param ptr - Array being converted in-place
 * @param count - Number of elements being processed in the array
 * @param size - Size of each element
 * @param cmp - Comparator function
 */
void make_max_heap                    (void *ptr,
                                       size_t count,
                                       size_t size,
                                       int (*cmp)(const void *, const void *));

/**
 * Sorts the array in-place using a heap
 *
 * @param ptr - Array being sorted in-place
 * @param count - Number of elements being processed in the array
 * @param size - Size of each element
 * @param cmp - Comparator function
 */
void heapsort                         (void *ptr,
                                       size_t count,
                                       size_t size,
                                       int (*cmp)(const void *, const void *));

#endif