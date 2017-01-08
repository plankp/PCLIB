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

#ifndef __NAMED_PARAMS_H__
#define __NAMED_PARAMS_H__

/**
 * Creates a function prototype for a named parameter and its dependencies.
 * This is achieved by passing a struct as its parameter instead and using
 * the ability of compound literals in C (or C++). If this is called in C++,
 * the linkage of the function is set to C (extern "C").
 *
 * To define a function prototype called sum that takes two integers (x, and
 * y) and returns an integer:
 *
 *    NAMED_PARAM_FUNC_PROTO(int, sum, int x; int y);
 *
 * Notice the parameters are not separated with commas but are separated
 * with semi colons instead.
 *
 * @param ret The return type of the function
 * @param name The name of the function
 * @param param The parameters of the function
 */
#ifdef __cplusplus
#define NAMED_PARAM_FUNC_PROTO(ret, name, param) \
		struct __##name##_$npp__ { param; }; \
		extern "C" ret name (struct __##name##_$npp__);
#else
#define NAMED_PARAM_FUNC_PROTO(ret, name, param) \
		struct __##name##_$npp__ { param; }; \
		extern ret name (struct __##name##_$npp__);
#endif

/**
 * Defines a named parameter function. This macro is the one that requires
 * an implementation to be filled in meaning it expects a function body.
 * The named parameters are accessed as attributes of a variable named _.
 *
 * A function can look like this (assuming the named parameters are x and
 * y)
 *
 *     int DEF_NAMED_PARAM_FUNC(sum) {
 *         return _.x + _.y;
 *     }
 *
 * Notice the return type is written before the macro and the parameters
 * are not spelled out.
 *
 * NAMED_PARAM_FUNC_PROTO MUST BE CALLED BEFORE THIS AT SOME POINT
 * (preferrably inside a header file)
 *
 * @param name The name of the function
 *
 * @body This macro expects a method body
 */
#define DEF_NAMED_PARAM_FUNC(name) \
		name (struct __##name##_$npp__ _)

/**
 * Dispatches a named parameter function. The parameters to the function
 * can be in both named style, or positional style.
 *
 * Here is an example of both styles calling a function called sum. It
 * returns an int and takes two integers as x and y.
 *
 * Named style:
 *
 *    CALL_NAMED_PARAM_FUNC(sum, .x = 1, .y = 2)
 *
 * Positional style:
 *
 *    CALL_NAMED_PARAM_FUNC(sum, 1, 2)
 *
 * @param name The name of the function
 * @varargs The parameters of the function
 *
 * @returns The result of the function
 */
#define CALL_NAMED_PARAM_FUNC(name, ...) \
		name ((struct __##name##_$npp__) { __VA_ARGS__ })

#endif
