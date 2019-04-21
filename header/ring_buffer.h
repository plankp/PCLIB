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

#ifndef __RING_BUFFER_H__
#define __RING_BUFFER_H__

#include <stddef.h>
#include <stdbool.h>

typedef struct ring_buffer
{
  size_t blk;
  char * lo;
  char * hi;
  char * start;
  char * end;
} ring_buffer;

/**
 * Initializes a ring buffer with the specified capacity
 *
 * @param buf - Pointer to an uninitialized ring buffer
 * @param cnt - Capacity
 * @param data_size - Size of each element
 *
 * @return true if cnt and data_size are more than zero
 */
bool init_ringbuf         (ring_buffer * buf, size_t cnt, size_t data_size);

/**
 * Frees a ring buffer, making it the same as uninitialized
 *
 * @param buf - Pointer to an initialized ring buffer
 */
void free_ringbuf         (ring_buffer * buf);

/**
 * Clears the ring buffer
 *
 * @param buf - Pointer to an initialized ring buffer
 */
void ringbuf_clear        (ring_buffer * buf);

/**
 * Returns the size of the ring buffer
 *
 * @param buf - Pointer to initialized ring buffer
 *
 * @return size of the ring buffer
 */
size_t ringbuf_size       (const ring_buffer * buf);

/**
 * Returns the capacity of the ring buffer
 *
 * @param buf - Pointer to initialized ring buffer
 *
 * @return capacity of the ring buffer
 */
size_t ringbuf_capacity   (const ring_buffer * buf);

/**
 * Checks if ring buffer is empty
 *
 * @param buf - Pointer to initialized ring buffer
 *
 * @return true if ring buffer is empty
 */
bool ringbuf_empty        (const ring_buffer * buf);

/**
 * Tries to append an element to the logical end of the ring buffer.
 *
 * @param buf - Pointer to initialized ring buffer
 * @param data - Pointer to element being added
 *
 * @return true if element was successfully added
 */
bool ringbuf_offer        (ring_buffer *restrict buf, const void *restrict data);

/**
 * Tries to remove an element from the logical head of the ring buffer.
 *
 * @param buf - Pointer to initialized ring buffer
 * @param out - Pointer that will be filled with the removed value; ignored if
 *              NULL
 *
 * @return true if buffer was not empty and element was successfully removed
 */
bool ringbuf_poll         (ring_buffer *restrict buf, void *restrict out);

/**
 * Returns a pointer to the next element to be polled.
 *
 * @param buf - Pointer to initialized ring buffer
 *
 * @return NULL if buffer is empty
 */
void const *ringbuf_peek  (const ring_buffer * buf);

/**
 * Iterates through the ring buffer in order of insertion
 *
 * @param buf - Pointer to initialized ring buffer
 * @param it - An action to be performed on each pair
 */
void ringbuf_foreach      (const ring_buffer * buf, void (*it)(const void *));

#endif