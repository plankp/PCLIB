#include "heap.h"

#include <time.h>
#include <stdio.h>

static
int cmp_int
(const void *a, const void *b)
{
  return *((const int *) a) - *((const int *) b);
}

int main
(int argc, char **argv)
{
  static int list_a[] =
  {
    49, 13, 51, 149, 80, 378, 338, 180, 191, 70, 215, 397, 143, 163, 76, 224, 308, 21, 55, 182,
    150, 200, 93, 2, 238, 268, 309, 211, 121, 4, 342, 126, 253, 324, 18, 208, 245, 337, 194,
    283, 135, 201, 399, 125, 288, 340, 370, 101, 301, 36, 398, 394, 41, 317, 310, 6, 11, 382,
    31, 221, 276, 219, 388, 391, 23, 34, 184, 290, 95, 282, 159, 188, 133, 162, 277, 330, 96, 328,
    63, 239, 355, 246, 71, 33, 145, 371, 102, 357, 234, 306, 92, 189, 205, 42, 250, 154, 202, 59,
    349, 1, 380, 7, 47, 48, 387, 87, 140, 119, 105, 123, 91, 127, 358, 269, 345, 199, 303, 344, 129,
    164, 296, 111, 165, 362, 137, 252, 68, 172, 67, 336, 280, 225, 356, 390, 15, 326, 192, 148, 352,
    45, 231, 247, 187, 177, 179, 174, 270, 85, 323, 130, 20, 228, 275, 230, 8, 46, 178, 298, 104,
    347, 120, 196, 374, 216, 116, 248, 297, 27, 292, 147, 40, 26, 72, 392, 223, 325, 265, 62, 295, 132,
    305, 60, 220, 312, 110, 98, 369, 396, 43, 156, 346, 74, 379, 73, 257, 316, 400, 343, 198, 213
  };

  puts("Original: ");
  for (size_t i = 0; i < sizeof list_a / sizeof *list_a; ++i)
  {
    printf("%d ", list_a[i]);
  }
  puts("");

  clock_t start = clock();
  heapsort(list_a, sizeof list_a / sizeof *list_a, sizeof *list_a, &cmp_int);
  clock_t stop = clock();

  puts("Sorted (heapsort): ");
  for (size_t i = 0; i < sizeof list_a / sizeof *list_a; ++i)
  {
    printf("%d ", list_a[i]);
  }
  printf("\nElapsed %lf\n", (stop - start) * 1000.0 / CLOCKS_PER_SEC);
  return 0;
}