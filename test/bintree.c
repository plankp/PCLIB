#include "bintree.h"

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

uint64_t
djb2_hash(char *key)
{
	uint64_t hash = 5381;
	int c;
	while (c = *key++)
	{
		hash = (hash << 5) + hash + c;
	}
	return hash;
}

STR_BINTREE_GENERATE(tr, int, djb2_hash);

int
main (void)
{
	BINTREE_TYPE(tr) d = BINTREE_INIT(tr, "d", 0);

	{
		BINTREE_TYPE(tr) a = BINTREE_INIT(tr, "a", 0);
		BINTREE_TYPE(tr) b = BINTREE_INIT(tr, "b", 0);
		BINTREE_TYPE(tr) c = BINTREE_INIT(tr, "c", 0);
		BINTREE_TYPE(tr) e = BINTREE_INIT(tr, "e", 0);
		BINTREE_TYPE(tr) f = BINTREE_INIT(tr, "f", 0);
		BINTREE_TYPE(tr) g = BINTREE_INIT(tr, "g", 0);

		BINTREE_INSERT(tr, &d, &a);
		BINTREE_INSERT(tr, &d, &b);
		BINTREE_INSERT(tr, &d, &c);

		BINTREE_INSERT(tr, &d, &e);
		BINTREE_INSERT(tr, &d, &f);
		BINTREE_INSERT(tr, &d, &g);
	}
	return 0;
}
