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

#ifndef __OPTIONAL_H__
#define __OPTIONAL_H__

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct optional_t
{
	const _Bool f;
	const void *d;
} optional_t;

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Creates a new optional value. If this value is equal to NULL, this optional
 * is set to none. In other words, absence of value.
 *
 * @param value The value being wrapped
 *
 * @returns The null wrapper
 */
optional_t init_optional (void *value);

/**
 * Checks to see if the optional has a value or not
 *
 * @param opt The optional value
 *
 * @returns True if value exist, false otherwise
 */
_Bool has_value (optional_t *opt);

/**
 * Checks to see if the optional is none
 *
 * @param opt The optional value
 *
 * @returns True if value does not exist, false otherwise
 */
_Bool is_empty (optional_t *opt);

/**
 * Returns the value of the optional wrapper. Returns NULL if optional was set
 * to none.
 *
 * @param opt The optional value
 *
 * @returns The underlying value. NULL if value is absent.
 */
const void *get_optional (optional_t *opt);

#ifdef __cplusplus
}
#endif

#endif
