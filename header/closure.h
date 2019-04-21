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

#ifndef __CLOSURE_H__
#define __CLOSURE_H__

#include <stddef.h>
#include <stdbool.h>

typedef struct closure closure;

/**
 * Constructs a new closure object with the specified behaviour and data space.
 * Data space, if applicable, is zeroed out during this call.
 *
 * @param fptr - Behaviour of the closure when being applied/invoked
 * @param data_size - Size of the data space, 0 if no data space is needed
 *
 * @return NULL if closure object cannot be allocated
 */
closure *new_closure      (void *(*fptr)(closure *, size_t, void *),
                           size_t data_size);

/**
 * Destroys a closure object
 *
 * @param clos - Closure object being destroyed
 */
void delete_closure       (closure *clos);

/**
 * Copies data into data space of closure object
 *
 * @param clos - Closure object whos data space is being changed
 * @param buf - Source of data
 * @param data_size - Number of bytes to copy
 */
void closure_set_data     (closure *restrict clos,
                           const void *restrict buf,
                           size_t data_size);

/**
 * Retrieves the pointer to the head of the data space
 *
 * @param clos - Closure object whos data space is being retrieved
 *
 * @return data space of the closure
 */
void *closure_get_data    (closure *clos);

/**
 * Applies/invokes the closure object by performing the behaviour defined.
 *
 * @param clos - Closure object being applied/invoked
 * @param sz - Number of bytes of argument buffer
 * @param argv - The argument buffer
 *
 * @return result of the behaviour tied to the closure
 */
void *closure_apply     (closure *clos,
                         size_t sz,
                         void *argv);

#endif