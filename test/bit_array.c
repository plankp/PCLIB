#include "bit_array.h"

#include <stdio.h>
#include <assert.h>
#include <stddef.h>

void
default_walker (bool b)
{
	printf("%d ", b);
}

int
main (void)
{
	bit_array arr;
	assert(init_bitarr(&arr, 10));

	// Set the first three bits
	bitarr_set(&arr, 0);
	bitarr_set(&arr, 1);
	bitarr_set(&arr, 2);

	// Set and then toggle the 8th bit
	bitarr_set(&arr, 7);
	bitarr_toggle(&arr, 7);

	// Print out the status of all bits
	printf("Should have three true bits\n");
	bitarr_foreach(&arr, &default_walker);
	printf("\n");

	// Perform binary NOT on the array
	bit_array arr1;
	init_cpy_bitarr(&arr1, &arr);
	bitarr_not(&arr1);

	// Print out the status of all bits
	printf("Should NOT have three true bits\n");
	bitarr_foreach(&arr1, &default_walker);
	printf("\n");

	// Perform binary OR on the two arrays
	bitarr_or(&arr, &arr1);

	// Print out the status of all bits
	printf("Should be all true bits\n");
	bitarr_foreach(&arr, &default_walker);
	printf("\n");

	// Clear the array
	bitarr_clear(&arr);

	// Print out the status of all bits
	printf("Should be all false bits\n");
	bitarr_foreach(&arr, &default_walker);
	printf("\n");

	// Initialize arr2 to a larger size than arr
	bit_array arr2;
	assert(init_bitarr(&arr2, 50));
	bitarr_not(&arr2);

	printf("\n");
	bitarr_foreach(&arr2, &default_walker);
	printf("\n");
	bitarr_foreach(&arr1, &default_walker);
	printf(" (and)\n");

	bitarr_and(&arr2, &arr1);
	bitarr_foreach(&arr2, &default_walker);
	printf("\n");

	free_bitarr(&arr1);
	free_bitarr(&arr2);

	free_bitarr(&arr);
}
