#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

#include "src/infrastructure/http/http.h"
#include "src/utils/hash_map/hash_map.h"

int main() {
  printf("=== hello \n");
  hash_map *m = new_hash_map();
  printf("=== put start!!! \n");
  put_to_hash_map(m, "sunjin", "aaaa");
  put_to_hash_map(m, "aaa", "hogehoge");

  printf("sunjin value is %s\n", get_value_from_hash_map(m, "sunjin"));

  int result = http_serve();
  if (result < 0) {
    printf("Error: Failed serve\n");
    return result;
  }

  return 0;
}
