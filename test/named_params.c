#include "named_params.h"

#include <stdio.h>

NAMED_PARAM_FUNC_PROTO(int, sum_of, int x; int y);

int DEF_NAMED_PARAM_FUNC(sum_of) {
	return _.x + _.y;
}

int
main (void)
{
	printf("Using named params (.x = 1, .y = 2): %d\n",
			CALL_NAMED_PARAM_FUNC(sum_of, .x = 1, .y = 2));
	printf("Using positional params (1, 2): %d\n",
			CALL_NAMED_PARAM_FUNC(sum_of, 1, 2));
	return 0;
}
