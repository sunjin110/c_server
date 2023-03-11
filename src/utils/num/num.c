#include <stdio.h>
#include <math.h>

extern int num_digit(size_t num) {
    return (int)log10(num) + 1;
}
