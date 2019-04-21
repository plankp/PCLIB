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

bool init_ringbuf         (ring_buffer * buf, size_t cnt, size_t data_size);
void free_ringbuf         (ring_buffer * buf);
void ringbuf_clear        (ring_buffer * buf);

size_t ringbuf_size       (const ring_buffer * buf);
size_t ringbuf_capacity   (const ring_buffer * buf);
bool ringbuf_empty        (const ring_buffer * buf);

bool ringbuf_offer        (ring_buffer *restrict buf, const void *restrict data);
bool ringbuf_poll         (ring_buffer *restrict buf, void *restrict out);
void const *ringbuf_peek  (const ring_buffer * buf);

void ringbuf_foreach      (const ring_buffer * buf, void (*it)(const void *));

#endif