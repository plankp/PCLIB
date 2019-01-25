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

#include "string_buffer.h"

#include <stdlib.h>
#include <string.h>

void init_strbuf
(string_buffer * const buffer)
{
  buffer->len = 0;
  buffer->cap = 0;
  buffer->mem = NULL;
}

void free_strbuf
(string_buffer * const buffer)
{
  if (buffer->cap > 0)
  {
    free(buffer->mem);

    buffer->len = 0;
    buffer->cap = 0;
    buffer->mem = NULL;
  }
}

char * strbuf_copy
(string_buffer * const buffer)
{
  size_t length = buffer->len;
  char * cpy = malloc((length + 1) * sizeof(char));
  memcpy(cpy, buffer->mem, length);
  cpy[length + 1] = '\0';
  return cpy;
}

char * cpy_free_strbuf
(string_buffer * const buffer)
{
  size_t length = buffer->len;
  if (length == 0)
  {
    /* allocate an empty string */
    char * cpy = calloc(1, sizeof(char));
    free_strbuf(buffer);
    return cpy;
  }

  /* internal buffer is well formed, so just return that */
  char * ptr = buffer->mem;
  buffer->len = 0;
  buffer->cap = 0;
  buffer->mem = NULL;
  return ptr;
}

void strbuf_clear
(string_buffer * const buffer)
{
  buffer->mem[buffer->len = 0] = '\0';
}

void strbuf_compact
(string_buffer * const buffer)
{
  size_t len = buffer->len;
  if (buffer->cap == len)
  {
    /* length already equals to capacity */
    return;
  }

  if (len == 0)
  {
    /* equivalent operation as free_strbuf */
    free_strbuf(buffer);
    return;
  }

  char * new_mem = realloc(buffer->mem, (len + 1) * sizeof(char));
  if (new_mem != NULL)
  {
    /* update cap if memory is resized */
    buffer->cap = len;
    buffer->mem = new_mem;
  }

  /* if realloc failed, continue using the original buffer */
}

bool strbuf_ensure_capacity
(string_buffer * const buffer, size_t n)
{
  if (buffer->cap >= n)
  {
    return true;
  }

  /* resize buffer to at least n */
  size_t new_cap = (((n + 1) / 16) + 1) * 16;
  char * new_mem = realloc(buffer->mem, new_cap * sizeof(char));
  if (new_mem == NULL)
  {
    return false;
  }

  buffer->cap = new_cap;
  buffer->mem = new_mem;
  return true;
}

bool strbuf_append_ch
(string_buffer * const buffer, char ch)
{
  if (!strbuf_ensure_capacity(buffer, buffer->len + 1))
  {
    return false;
  }

  buffer->mem[buffer->len++] = ch;
  buffer->mem[buffer->len] = '\0';
  return true;
}

bool strbuf_append_nstr
(string_buffer *restrict const buffer, char const *restrict str, size_t const count)
{
  if (!strbuf_ensure_capacity(buffer, buffer->len + count))
  {
    return false;
  }

  memcpy(&buffer->mem[buffer->len], str, count);
  buffer->mem[buffer->len += count] = '\0';
  return true;
}

bool strbuf_append_str
(string_buffer *restrict const buffer, char const *restrict str)
{
  return strbuf_append_nstr(buffer, str, strlen(str));
}

size_t strbuf_size
(string_buffer * const buffer)
{
  return buffer->len;
}

size_t strbuf_capacity
(string_buffer * const buffer)
{
  return buffer->cap;
}

char * strbuf_data
(string_buffer * const buffer)
{
  return buffer->mem == NULL ? "" : buffer->mem;
}