#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "hashmap.h"

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

static
int
default_walker(char *key, char *val)
{
	printf("%s => %s\n", key, val);
}

STR_MAP_GENERATE(hash, char *, djb2_hash);

int
main (void)
{
	MAP_TYPE(hash) *map = calloc(1, sizeof (MAP_TYPE(hash)));
	MAP_PUT_PAIR(hash, map, "A", "Apple");
	MAP_PUT_PAIR(hash, map, "B", "Ball");
	MAP_PUT_PAIR(hash, map, "C", "Cat");
	MAP_PUT_PAIR(hash, map, "MLG", "Noscoped");
	printf("MLG => %s\n\n", MAP_GET_VALUE(hash, map, "MLG"));
	MAP_FOR_EACH(hash, map, &default_walker);
	MAP_REMOVE_VALUE(hash, map, "A");
	MAP_PUT_PAIR(hash, map, "B", "Bat");
	printf("\n");
	MAP_FOR_EACH(hash, map, &default_walker);
	printf("Get hui (default \"A\") %s\n", MAP_GET_OR_DEFAULT(hash, map, "hui", "A"));
	printf("Does A exist? %d\n", MAP_HAS_KEY(hash, map, "A"));
	printf("Does B exist? %d\n", MAP_HAS_KEY(hash, map, "B"));
	free(map);
	return 0;
}
