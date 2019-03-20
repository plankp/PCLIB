#include "tree_multiset.h"

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
(void const * key_slot, size_t matches)
{
  /* inserting (char const *, int):
   * - key: char const * const *
   * - value: int const *
   */
  char const * const * key = key_slot;
  printf("%s(%zu)\n", *key, matches);
}

int main
(int argc, char **argv)
{
  tree_multiset tree;
  init_tmset(&tree, &string_cmp, sizeof(char const *));

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
    tmset_put(&tree, &test_data[i]);
  }

  char const * str = "Beta";
  tmset_put_if_absent(&tree, &str);

  tmset_put_if_absent(&tree, &str);

  tmset_foreach(&tree, &default_walker);
  printf("Tree size: %zu\n\n", tmset_size(&tree));

  str = "Gamma";
  printf("Tree has 'Gamma'?: %d\n", tmset_has_key(&tree, &str));

  str = "Alpha";
  printf("Tree has %zu 'Alpha's\n", tmset_count_matches(&tree, &str));

  str = "Foo";
  printf("Tree has %zu 'Foo's\n\n", tmset_count_matches(&tree, &str));

  str = "Alpha";
  tmset_remove_count(&tree, &str, 1);
  tmset_foreach(&tree, &default_walker);
  printf("Tree size: %zu\n\n", tmset_size(&tree));

  str = "PzrhlPLqET";
  tmset_remove(&tree, &str);
  tmset_foreach(&tree, &default_walker);
  printf("Tree size: %zu\n\n", tmset_size(&tree));

  str = "rYxhygHpTi";
  tmset_remove(&tree, &str);
  tmset_foreach(&tree, &default_walker);
  printf("Tree size: %zu\n\n", tmset_size(&tree));

  str = "a";
  printf("\nThings lesser than \"a\":\n");
  tmset_foreach_lt(&tree, &str, &default_walker);
  printf("\nThings greater than \"a\":\n");
  tmset_foreach_gt(&tree, &str, &default_walker);

  free_tmset(&tree);
  return 0;
}