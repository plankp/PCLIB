#include "tree_set.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

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
(void const * key_slot)
{
  char const * const * key = key_slot;
  printf("%s\n", *key);
}

int main
(int argc, char **argv)
{
  tree_set tree;
  init_tset(&tree, &string_cmp, sizeof(char const *));

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
    tset_put(&tree, &test_data[i]);
  }

  char const * str = "Beta";
  assert(tset_put_if_absent(&tree, &str) == true);

  assert(tset_put_if_absent(&tree, &str) == false);

  tset_foreach(&tree, &default_walker);
  printf("Tree size: %zu\n\n", tset_size(&tree));

  str = "Gamma";
  printf("Tree has 'Gamma'?: %d\n", tset_has_key(&tree, &str));

  str = "Foo";
  printf("Tree has 'Foo'?: %d\n", tset_has_key(&tree, &str));

  str = "Alpha";
  tset_remove(&tree, &str);
  tset_foreach(&tree, &default_walker);
  printf("Tree size: %zu\n\n", tset_size(&tree));

  str = "PzrhlPLqET";
  tset_remove(&tree, &str);
  tset_foreach(&tree, &default_walker);
  printf("Tree size: %zu\n\n", tset_size(&tree));

  str = "rYxhygHpTi";
  tset_remove(&tree, &str);
  tset_foreach(&tree, &default_walker);
  printf("Tree size: %zu\n\n", tset_size(&tree));

  str = "a";
  printf("\nThings lesser than \"a\":\n");
  tset_foreach_lt(&tree, &str, &default_walker);
  printf("\nThings greater than \"a\":\n");
  tset_foreach_gt(&tree, &str, &default_walker);

  free_tset(&tree);
  return 0;
}