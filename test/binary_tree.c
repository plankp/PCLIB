#include "binary_tree.h"

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
(void const * key_slot, size_t matches, void const * values)
{
  /* inserting (char const *, int):
   * - key: char const * const *
   * - value: int const *
   */
  char const * const * key = key_slot;
  int const *ints = values;
  printf("%s(%zu): ", *key, matches);
  for (size_t i = 0; i < matches; ++i)
  {
    printf("%d ", ints[i]);
  }
  puts("");
}

int main
(int argc, char **argv)
{
  binary_tree tree;
  init_bintree(&tree, &string_cmp, sizeof(char const *), sizeof(int));

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
    bintree_put(&tree, &test_data[i], &i);
  }

  int tmp = 100;
  char const * str = "Beta";
  bintree_put_if_absent(&tree, &str, &tmp);

  tmp = 101;
  bintree_put_if_absent(&tree, &str, &tmp);

  bintree_foreach(&tree, &default_walker);
  printf("Tree size: %zu\n\n", bintree_size(&tree));

  str = "Gamma";
  printf("Tree has 'Gamma'?: %d\n", bintree_has_key(&tree, &str));

  str = "Alpha";
  printf("Tree has %zu 'Alpha's\n", bintree_count_matches(&tree, &str));

  str = "Foo";
  printf("Tree has %zu 'Foo's\n\n", bintree_count_matches(&tree, &str));

  str = "Alpha";
  bintree_remove(&tree, &str);
  bintree_foreach(&tree, &default_walker);
  printf("Tree size: %zu\n\n", bintree_size(&tree));

  str = "PzrhlPLqET";
  bintree_remove(&tree, &str);
  bintree_foreach(&tree, &default_walker);
  printf("Tree size: %zu\n\n", bintree_size(&tree));

  str = "rYxhygHpTi";
  bintree_remove(&tree, &str);
  bintree_foreach(&tree, &default_walker);
  printf("Tree size: %zu\n\n", bintree_size(&tree));

  str = "Beta";
  printf("Beta --> %d\n", *(int const *) bintree_get(&tree, &str, NULL));

  str = "a";
  printf("\nThings lesser than \"a\":\n");
  bintree_foreach_lt(&tree, &str, &default_walker);
  printf("\nThings greater than \"a\":\n");
  bintree_foreach_gt(&tree, &str, &default_walker);

  free_bintree(&tree);
  return 0;
}