#include "array_list.h"

#include <assert.h>
#include <stdio.h>
#include <stddef.h>

static
void default_walker
(const void * el)
{
	printf("%d ", *(int *) el);
}

int main
(void)
{
	array_list list;
	assert(("Initialize array list of ints", init_arrlist(&list, sizeof(int))));

	size_t i;
	for (i = 0; i < 10; ++i)
	{
		int val = i + 1;
		arrlist_add(&list, &val);
	}
	arrlist_foreach(&list, &default_walker);
	printf("\nSize is %zu\n", arrlist_size(&list));

	int val = 10;
	arrlist_remove_last(&list, &val);
	arrlist_remove_last(&list, &val);
	arrlist_remove_last(&list, &val);
	arrlist_compact(&list);
	assert(("val should be 8", val == 8));

	arrlist_foreach(&list, &default_walker);
	printf("\nSize is %zu\n", arrlist_size(&list));

	arrlist_remove(&list, 0, NULL);
	arrlist_remove(&list, 0, NULL);
	arrlist_foreach(&list, &default_walker);
	printf("\nSize is %zu\n", arrlist_size(&list));

	val = 0;
	arrlist_insert(&list, 0, &val);
	arrlist_foreach(&list, &default_walker);
	printf("\nSize is %zu\n", arrlist_size(&list));

	arrlist_clear(&list);
	arrlist_foreach(&list, &default_walker);
	printf("\nSize is %zu\n", arrlist_size(&list));

	free_arrlist(&list);
	printf("\nDONE\n");
	return 0;
}
