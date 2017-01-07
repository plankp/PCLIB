#include "optional.h"

optional_t
init_optional(void *value)
{
	if (value == NULL)
	{
		return (optional_t) {
			.f=false, .d=NULL
		};
	}
	return (optional_t) {
		.f=true, .d=value
	};
}

_Bool
has_value(optional_t *opt)
{
	return opt->f;
}

_Bool
is_empty(optional_t *opt)
{
	return !opt->f;
}

const void *
get_optional(optional_t *opt)
{
	return has_value(opt) ? opt->d : NULL;
}
