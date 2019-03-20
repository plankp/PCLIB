#include "tree_map.h"

#include <stdio.h>
#include <string.h>

static
int string_cmp
(void const * a, void const * b)
{
  char const * const * strA = a;
  char const * const * strB = b;

  return strcmp(*strA, *strB);
}

static
void default_walker
(void const * key_slot, void const * values)
{
  /* inserting (char const *, int):
   * - key: char const * const *
   * - value: int const *
   */
  char const * const * key = key_slot;
  int const *ints = values;
  printf("%s: %d\n", *key, *ints);
}

int main
(int argc, char **argv)
{
  tree_map tree;
  init_tmap(&tree, &string_cmp, sizeof(char const *), sizeof(int));

  static char const * const test_data[] =
  {
    "PzrhlPLqET",
    "SEpxydXyeY",
    "RfQzoZeUWt",
    "rYxhygHpTi",
    "zuGxHdVzmr",
    "NhqWNkLvsP",
    "vfLMetsFpo",
    "xXCdPLDsfh",
    "piotAQrkQc",
    "qvTYTPaXty",

    "Alpha",
    "Alpha",
    "Gamma",
    "Zeta",
  };

  for (size_t i = 0; i < sizeof(test_data) / sizeof(char const *); ++i)
  {
    tmap_put(&tree, &test_data[i], &i);
  }

  int tmp = 100;
  char const * str = "Beta";
  tmap_put_if_absent(&tree, &str, &tmp);

  tmp = 101;
  tmap_put_if_absent(&tree, &str, &tmp);

  tmap_foreach(&tree, &default_walker);
  printf("Tree size: %zu\n\n", tmap_size(&tree));

  str = "Gamma";
  printf("Tree has 'Gamma'?: %d\n", tmap_has_key(&tree, &str));

  str = "Alpha";
  printf("Tree has 'Alpha'?: %d\n", tmap_has_key(&tree, &str));

  str = "Foo";
  printf("Tree has 'Foo'?: %d\n\n", tmap_has_key(&tree, &str));

  str = "Alpha";
  tmap_remove(&tree, &str);
  tmap_foreach(&tree, &default_walker);
  printf("Tree size: %zu\n\n", tmap_size(&tree));

  str = "PzrhlPLqET";
  tmap_remove(&tree, &str);
  tmap_foreach(&tree, &default_walker);
  printf("Tree size: %zu\n\n", tmap_size(&tree));

  str = "rYxhygHpTi";
  tmap_remove(&tree, &str);
  tmap_foreach(&tree, &default_walker);
  printf("Tree size: %zu\n\n", tmap_size(&tree));

  str = "Beta";
  printf("Beta --> %d\n", *(int const *) tmap_get(&tree, &str));

  str = "a";
  printf("\nThings lesser than \"a\":\n");
  tmap_foreach_lt(&tree, &str, &default_walker);
  printf("\nThings greater than \"a\":\n");
  tmap_foreach_gt(&tree, &str, &default_walker);

  free_tmap(&tree);
  return 0;
}