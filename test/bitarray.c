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

	// Set and then toggle the 8th bit
	bitarray_set(&arr, 7);
	bitarray_toggle(&arr, 7);

	// Print out the status of all bits
	printf("Should have three true bits\n");
	bitarray_for_each(&arr, &default_walker);
	printf("\n");

	// Perform binary NOT on the array
	bitarray_t arr1 = bitarray_not(&arr);

	// Print out the status of all bits
	printf("Should NOT have three true bits\n");
	bitarray_for_each(&arr1, &default_walker);
	printf("\n");

	// Perform binary OR on the two arrays
	bitarray_t arr2 = bitarray_or(&arr, &arr1);

	// Print out the status of all bits
	printf("Should be all true bits\n");
	bitarray_for_each(&arr2, &default_walker);
	printf("\n");

	// Clear the array
	bitarray_clear(&arr2);

	// Print out the status of all bits
	printf("Should be all false bits\n");
	bitarray_for_each(&arr2, &default_walker);
	printf("\n");

	clean_bitarray(&arr);
	clean_bitarray(&arr1);
	clean_bitarray(&arr2);
}
