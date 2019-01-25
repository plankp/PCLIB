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

#ifndef __STRING_BUFFER_H__
#define __STRING_BUFFER_H__

#include <stddef.h>
#include <stdbool.h>

/**
 * The growth function for the string buffer.
 *
 * By default, the capacity is scaled to the next multiple of 16.
 *
 * @param n - The precomputed new capacity
 *
 * @return value >= n
 */
#ifndef STRBUF_GROW /* (size_t n) */
#define STRBUF_GROW(n) ((((n + 1) / 16) + 1) * 16)
#endif

typedef struct string_buffer
{
  size_t len;
  size_t cap;
  char * mem;
} string_buffer;

/**
 * Initializes an empty string buffer
 *
 * @param buffer - Pointer to an uninitialized string buffer
 */
void init_strbuf            (string_buffer * const buffer);

/**
 * Frees a string buffer, making it the same as uninitialized.
 *
 * @param buffer - Pointer to initialized string buffer
 */
void free_strbuf            (string_buffer * const buffer);


/**
 * Duplicates the internal string.
 *
 * @param buffer - Pointer to initialized string buffer
 *
 * @return duplicated string, must be freed later
 */
char * strbuf_copy          (string_buffer * const buffer);

/**
 * Duplicates the internal string then frees the string buffer, making it the
 * same as uninitialized.
 *
 * @param buffer - Pointer to initialized string buffer
 *
 * @return duplicated string, must be freed later
 */
char * cpy_free_strbuf      (string_buffer * const buffer);

/**
 * Clears the string buffer by setting the size to zero. To release unused
 * parts of the buffer, call strbuf_compact immediately after.
 *
 * @param buffer - Pointer to initialized string buffer
 */
void strbuf_clear           (string_buffer * const buffer);

/**
 * Attempts to release unused parts of the buffer; in other words, tries to
 * make the capacity of the buffer the same as the size of the buffer.
 *
 * @param buffer - Pointer to initialized string buffer
 */
void strbuf_compact         (string_buffer * const buffer);

/**
 * Ensures the capacity is at least n.
 *
 * @param buffer - Pointer to initialized string buffer
 * @param n - New minimum capacity of the buffer
 *
 * @return true if capacity was already at least n or the internal buffer was
 * able to be allocated successfully
 */
bool strbuf_ensure_capacity (string_buffer * const buffer, size_t n);

/**
 * Appends a character to the end of the buffer
 *
 * @param buffer - Pointer to initialized string buffer
 * @param ch - Character being appended
 *
 * @return true if operation succedded
 */
bool strbuf_append_ch       (string_buffer * const buffer, char ch);

/**
 * Appends a string to the end of the buffer
 *
 * @param buffer - Pointer to initialized string buffer
 * @param str - String being appended
 *
 * @return true if operation succedded
 */
bool strbuf_append_str      (string_buffer * restrict const buffer,
                             char const * restrict str);

/**
 * Appends a specified number of characters of a string to the end of the
 * buffer
 *
 * @param buffer - Pointer to initialized string buffer
 * @param str - String
 * @param count - The number of characters being appended
 *
 * @return true if operation succedded
 */
bool strbuf_append_nstr     (string_buffer * restrict const buffer,
                             char const * restrict str,
                             size_t const count);

/**
 * Returns the size of the string buffer
 *
 * @param buffer - Pointer to initialized string buffer
 *
 * @return size of the string buffer
 */
size_t strbuf_size          (string_buffer * const buffer);

/**
 * Returns the capacity of the string buffer
 *
 * @param buffer - Pointer to initialized string buffer
 *
 * @return capacity of the string buffer
 */
size_t strbuf_capacity      (string_buffer * const buffer);

/**
 * Returns a pointer to the first character
 *
 * @param buffer - Pointer to initialized string buffer
 *
 * @return pointer to the first character, must not be freed
 */
char * strbuf_data          (string_buffer * const buffer);

#endif