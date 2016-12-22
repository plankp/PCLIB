#include "closure.h"

#include <stdio.h>

CLOSURE_GENERATE(t5, int, int, int p)
{
	return p * self;
}

int
main (void)
{
	CLOSURE_TYPE(t5) clos = CLOSURE_INIT(t5, 5);
	printf("5 * (1, 2, 3) = (%d, %d, %d)\n",
			CLOSURE_CALL(clos, 1),
			CLOSURE_CALL(clos, 2),
			CLOSURE_CALL(clos, 3));
	return 0;
}
