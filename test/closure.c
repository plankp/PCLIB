#include "closure.h"

#include <stdio.h>

CLOSURE_GENERATE(t5, int, int, int p)
{
	return p * *self;
}

SUPPLIER_GENERATE(sint, size_t, size_t)
{
	return (*self)++;
}

int
main (void)
{
	CLOSURE_TYPE(t5) clos = CLOSURE_INIT(t5, 5);
	printf("5 * (1, 2, 3) = (%d, %d, %d)\n",
			GENERIC_CALL(&clos, 1),
			GENERIC_CALL(&clos, 2),
			GENERIC_CALL(&clos, 3));

	SUPPLIER_TYPE(sint) sint = SUPPLIER_INIT(sint, 0);
	size_t i;
	for (i = 0; i < 10; ++i)
	{
		printf("%zu ", GENERIC_CALL(&sint));
	}
	printf("\n");
	return 0;
}
