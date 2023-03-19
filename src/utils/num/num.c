#include <math.h>
#include <stdio.h>

extern int num_digit(size_t num) {
  if (num == 0) {
    return 0;
  }
  return (int)log10(num) + 1;
}
