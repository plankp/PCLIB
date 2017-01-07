#include "bitarray.h"

#include <stdio.h>
#include <stddef.h>

loop_state_t
default_walker (int b)
{
	printf("%d ", b);
	return CONTINUE;
}

int
main (void)
{
	bitarray_t arr = init_bitarray(10);

	// Set the first three bits
	bitarray_set(&arr, 0);
	bitarray_set(&arr, 1);
	bitarray_set(&arr, 2);

	// Set and then toggle the 7th bit
	bitarray_set(&arr, 7);
	bitarray_toggle(&arr, 7);

	// Print out the status of all bits
	bitarray_for_each(&arr, &default_walker);
	printf("\n");

	// Clear all
	bitarray_clear(&arr);

	// Print out the status of all bits
	bitarray_for_each(&arr, &default_walker);
	printf("\n");

	clean_bitarray(&arr);
}
