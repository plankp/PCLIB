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

#include "ring_buffer.h"

#include <stdlib.h>
#include <string.h>

bool init_ringbuf
(ring_buffer * buf, size_t cnt, size_t data_size)
{
  if (cnt == 0 || data_size == 0) return false;

  void * mem = calloc(cnt, data_size);
  if (mem == NULL) return false;

  buf->blk = data_size;
  buf->start = mem;
  buf->end = mem + data_size * cnt;
  buf->lo = buf->hi = buf->start;
  return true;
}

void free_ringbuf
(ring_buffer * buf)
{
  if (buf->end != buf->start)
  {
    free(buf->start);
    buf->start = NULL;
    buf->end = NULL;

    ringbuf_clear(buf);
  }
}

void ringbuf_clear
(ring_buffer * buf)
{
  buf->lo = buf->hi = buf->start;
}

size_t ringbuf_size
(const ring_buffer * buf)
{
  if (buf->lo <= buf->hi) return (buf->hi - buf->lo) / buf->blk;

  /* hi actually has lower address then lo: (for example)
   *
   * start    hi       lo                end
   * +--------+--------+--------+--------+
   *
   * size is ((end - low) + (hi - start)) / block-size
   */
  return ((buf->end - buf->lo) + (buf->hi - buf->start)) / buf->blk;
}

size_t ringbuf_capacity
(const ring_buffer * buf)
{
  return (buf->end - buf->start) / buf->blk;
}

bool ringbuf_empty
(const ring_buffer * buf)
{
  return buf->hi == buf->lo;
}

bool ringbuf_offer
(ring_buffer *restrict buf, const void *restrict data)
{
  char * next = buf->hi + buf->blk;
  if (next >= buf->end) next = buf->start;
  if (next == buf->lo) return false;

  memcpy(buf->hi, data, buf->blk);
  buf->hi = next;
  return true;
}

bool ringbuf_poll
(ring_buffer *restrict buf, void *restrict out)
{
  if (ringbuf_empty(buf)) return false;

  if (out != NULL) memcpy(out, buf->lo, buf->blk);
  if ((buf->lo += buf->blk) >= buf->end) buf->lo = buf->start;
  return true;
}

void const *ringbuf_peek
(const ring_buffer * buf)
{
  return ringbuf_empty(buf) ? NULL : buf->lo;
}

void ringbuf_foreach
(const ring_buffer * buf, void (*it)(const void *))
{
  char *i = buf->lo;
  while (i != buf->hi)
  {
    it(i);
    if ((i += buf->blk) >= buf->end) i = buf->start;
  }
}