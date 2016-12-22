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
