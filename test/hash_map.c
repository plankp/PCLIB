#include "hash_map.h"

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct str_str_pair
{
	char const * key;
	char const * val;
} str_str_pair;

static
unsigned long key_hash
(void const * ptr)
{
	str_str_pair const * pair = ptr;
	char const * key = pair->key;
	unsigned long hash = 5381;
	int c;
	while ((c = *key++))
	{
		hash = (hash << 5) + hash + c;
	}
	return hash;
}

static
bool key_eql
(void const * ptrA, void const * ptrB)
{
	str_str_pair const * pairA = ptrA;
	str_str_pair const * pairB = ptrB;
	return strcmp(pairA->key, pairB->key) == 0;
}

static
void default_walker
(void const * ptr)
{
	str_str_pair const * pair = ptr;
	printf("%s => %s\n", pair->key, pair->val);
}

int
main (void)
{
	hash_map map;
	init_hmap(&map, &key_hash, &key_eql);

	hmap_put(&map, &(str_str_pair) { "A", "Apple" }, NULL);
	hmap_put(&map, &(str_str_pair) { "B", "Ball" }, NULL);
	hmap_put(&map, &(str_str_pair) { "C", "Cat" }, NULL);
	hmap_put(&map, &(str_str_pair) { "MLG", "Noscoped" }, NULL);
	printf("MLG => %s\n\n", ((str_str_pair *) hmap_get(&map, &(str_str_pair) { "MLG" }))->key);
	hmap_foreach(&map, &default_walker);
	hmap_remove(&map, &(str_str_pair) { "A" });
	hmap_put(&map, &(str_str_pair) { "B", "Bat" }, NULL);
	printf("\n");
	hmap_foreach(&map, &default_walker);
	printf("\nGet hui (default \"A\") %s\n",
			((str_str_pair *) hmap_get_or_default(&map, &(str_str_pair) { "hui", "A" }))->val);
	printf("Does A exist? %d\n", hmap_has_key(&map, &(str_str_pair) { "A" }));
	printf("Does B exist? %d\n", hmap_has_key(&map, &(str_str_pair) { "B" }));
	printf("\nFinal size of map: %zu\n", hmap_size(&map));
	free_hmap(&map);
	return 0;
}
