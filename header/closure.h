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

#ifndef __CLOSURE_H__
#define __CLOSURE_H__

/**
 * Base macro for generating all other function generating related macros
 *
 * Note: You should not call this macro directly since it will not be
 * compatible with all the other macros (except for GENERIC_CALL)
 */
#define RAW_CLOSURE_GENERATE(id, B, R, ...) \
	struct id##_closure_t \
	{ \
		R (*fptr)(B*, ##__VA_ARGS__); \
		B block; \
	}; \
 \
	static inline R __##id##_closure_func(B*, ##__VA_ARGS__); \
 \
	struct id##_closure_t \
	init_##id##_closure(B block) \
	{ \
		return (struct id##_closure_t) { \
			.fptr=&__##id##_closure_func, \
			.block=block \
		}; \
	} \
 \
	static inline \
	R \
	__##id##_closure_func(B *self, ##__VA_ARGS__)

// ----- Generators -----

/**
 * Generates a closure. The name closure is a bit misleading
 * since it cannot be mixed with the other closure types.
 *
 * @param id An unique name for the closure
 * @param B Block type. Typically a struct containing all
 * the captured variables. Referenced as self in the closure.
 * @param R Return type of the closure
 * @param ... The parameters
 *
 * @body The body of the closure is immediately supplied
 * after this call.
 */
#define CLOSURE_GENERATE(id, B, R, ...) \
		RAW_CLOSURE_GENERATE(clos_##id, B, R, ##__VA_ARGS__)

/**
 * Generates a consumer (a non-compatible void returning
 * closure)
 *
 * @param id An unique name for the closure
 * @param B Block type. Typically a struct containing all
 * the captured variables. Referenced as self in the closure.
 * @param ... The parameters
 *
 * @body The body of the closure is immediately supplied
 * after this call.
 */
#define CONSUMER_GENERATE(id, B, ...) \
		RAW_CLOSURE_GENERATE(cons_##id, B, void, ##__VA_ARGS__)

/**
 * Generates a predicate (a non-compatible boolean returning
 * closure)
 *
 * @param id An unique name for the closure
 * @param B Block type. Typically a struct containing all
 * the captured variables. Referenced as self in the closure.
 * @param ... The parameters
 *
 * @body The body of the closure is immediately supplied
 * after this call.
 */
#define PREDICATE_GENERATE(id, B, ...) \
		RAW_CLOSURE_GENERATE(pred_##id, B, _Bool, ##__VA_ARGS__)

/**
 * Generates a supplier (a non-compatible closure that takes
 * no parameters)
 *
 * @param id An unique name for the closure
 * @param B Block type. Typically a struct containing all
 * the captured variables. Referenced as self in the closure.
 * @param R Return type of the closure
 *
 * @body The body of the closure is immediately supplied
 * after this call.
 */
#define SUPPLIER_GENERATE(id, B, R) \
		RAW_CLOSURE_GENERATE(supp_##id, B, R)

// ----- Typedef-ish -----

/**
 * Returns the type of the closure (not raw_closure)
 * associated with the id
 *
 * @param id The unique name for the closure
 *
 * @returns The expanded name
 */
#define CLOSURE_TYPE(id) struct clos_##id##_closure_t

/**
 * Returns the type of the consumer associated with
 * the id
 *
 * @param id The unique name for the consumer
 *
 * @returns The expanded name
 */
#define CONSUMER_TYPE(id) struct cons_##id##_closure_t

/**
 * Returns the type of the predicate associated
 * with the id
 *
 * @param id The unique name for the predicate
 *
 * @returns The expanded name
 */
#define PREDICATE_TYPE(id) struct pred_##id##_closure_t

/**
 * Returns the type of the supplier associated
 * with the id
 *
 * @param id The unique name for the supplier
 *
 * @returns The expanded name
 */
#define SUPPLIER_TYPE(id) struct supp_##id##_closure_t

// ----- Initializers -----

#define CLOSURE_INIT(id, block) (init_clos_##id##_closure(block))

#define CONSUMER_INIT(id, block) (init_cons_##id##_closure(block))

#define PREDICATE_INIT(id, block) (init_pred_##id##_closure(block))

#define SUPPLIER_INIT(id, block) (init_supp_##id##_closure(block))

// ----- Apply -----

/**
 * Performs a call for any raw closure derived
 * closure
 *
 * @param self The raw closure
 * @param ... The parameters for the closure. Do
 * not pass anything in if the closure is a
 * supplier.
 *
 * @returns The result of the closure call.
 * Consumers will always return void.
 *
 */
#define GENERIC_CALL(self, ...) ((self)->fptr(&(self)->block, ##__VA_ARGS__))

#endif
