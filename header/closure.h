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

#define CLOSURE_GENERATE(id, B, R, ...) \
	struct id##_closure_t \
	{ \
		R (*fptr)(B, ##__VA_ARGS__); \
		B block; \
	}; \
\
	static inline R __##id##_closure_func(B, ##__VA_ARGS__); \
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
	__##id##_closure_func(B self, ##__VA_ARGS__) \

// Note: The user supplies the block body themselves

#define CLOSURE_TYPE(id) struct id##_closure_t

#define CLOSURE_INIT(id, block) (init_##id##_closure(block))

#define CLOSURE_CALL(self, ...) (self.fptr(self.block, ##__VA_ARGS__))

#endif
