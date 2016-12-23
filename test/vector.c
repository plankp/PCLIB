#include "vector.h"

#include <assert.h>
#include <stdio.h>
#include <stddef.h>

static inline
loop_state_t
default_walker(int el)
{
	printf("%d ", el);
	return CONTINUE;
}

INT_VEC_GENERATE(v);

int
main(void)
{
	VECTOR_TYPE(v) vec = VECTOR_INIT(v, 4);

	size_t i;
	for (i = 0; i < 10; ++i) VECTOR_PUSH(v, &vec, i + 1);
	VECTOR_FOR_EACH(v, &vec, &default_walker);
	printf("\nSize is %zu\n", VECTOR_SIZE(v, &vec));

	int val = 10;
	VECTOR_POP(v, &vec, &val);
	VECTOR_POP(v, &vec, &val);
	VECTOR_POP(v, &vec, &val);
	VECTOR_TRIM_UNUSED(v, &vec);
	assert(("val should be 8", val == 8));

	VECTOR_FOR_EACH(v, &vec, &default_walker);
	printf("\nSize is %zu\n", VECTOR_SIZE(v, &vec));

	VECTOR_REMOVE(v, &vec, 0, NULL);
	VECTOR_REMOVE(v, &vec, 0, NULL);
	VECTOR_FOR_EACH(v, &vec, &default_walker);
	printf("\nSize is %zu\n", VECTOR_SIZE(v, &vec));

	VECTOR_INSERT(v, &vec, 0, 0);
	VECTOR_FOR_EACH(v, &vec, &default_walker);
	printf("\nSize is %zu\n", VECTOR_SIZE(v, &vec));

	VECTOR_CLEAR(v, &vec);
	VECTOR_FOR_EACH(v, &vec, &default_walker);
	printf("\nSize is %zu\n", VECTOR_SIZE(v, &vec));

	VECTOR_CLEAN(v, &vec);
}
