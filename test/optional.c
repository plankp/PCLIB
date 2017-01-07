#include "optional.h"

#include <stdio.h>
#include <stddef.h>

int
main (void)
{
	// Testing empty values
	optional_t opt1 = init_optional(NULL);
	if (is_empty(&opt1)) printf("This value is empty!\n");

#ifdef __cplusplus
	optional_t opt2 = init_optional(const_cast<char*>("Abc"));
#else
	optional_t opt2 = init_optional("Abc");
#endif

	if (has_value(&opt2)) printf("Contains %s\n", get_optional(&opt2));

	return 0;
}
