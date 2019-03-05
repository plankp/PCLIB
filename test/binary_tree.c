#include "binary_tree.h"

#include <stdio.h>
#include <string.h>

static
int string_cmp
(void const * a, void const * b)
{
  char const * strA = a;
  char const * strB = b;

  return strcmp(strA, strB);
}

static
void default_walker
(void const * key, size_t matches, void const * values)
{
  /* inserting (char const *, int):
   * - key: char const *
   * - value: int const *
   */
  int const *ints = values;
  printf("%s(%zu): ", (char const *) key, matches);
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
  init_bintree(&tree, &string_cmp, sizeof(int));

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
    bintree_put(&tree, test_data[i], &i);
  }

  int tmp = 100;
  bintree_put_if_absent(&tree, "Beta", &tmp);

  tmp = 101;
  bintree_put_if_absent(&tree, "Beta", &tmp);

  bintree_foreach(&tree, &default_walker);
  printf("Tree size: %zu\n\n", bintree_size(&tree));

  printf("Tree has 'Gamma'?: %d\n", bintree_has_key(&tree, "Gamma"));
  printf("Tree has %zu 'Alpha's\n", bintree_count_matches(&tree, "Alpha"));
  printf("Tree has %zu 'Foo's\n\n", bintree_count_matches(&tree, "Foo"));

  bintree_remove(&tree, "Alpha");
  bintree_foreach(&tree, &default_walker);
  printf("Tree size: %zu\n\n", bintree_size(&tree));

  bintree_remove(&tree, "PzrhlPLqET");
  bintree_foreach(&tree, &default_walker);
  printf("Tree size: %zu\n\n", bintree_size(&tree));

  bintree_remove(&tree, "rYxhygHpTi");
  bintree_foreach(&tree, &default_walker);
  printf("Tree size: %zu\n\n", bintree_size(&tree));

  printf("Beta --> %d\n", *(int const *) bintree_get(&tree, "Beta", NULL));

  free_bintree(&tree);
  return 0;
}