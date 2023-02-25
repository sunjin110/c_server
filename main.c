#include <stdio.h>
#include "src/infrastructure/http/http.h"

int main() {
  int result = http_serve();
  if (result < 0) {
    printf("Error: Failed serve\n");
    return result;
  }

  return 0;
}
