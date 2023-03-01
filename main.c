#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

#include "src/infrastructure/http/http.h"
#include "src/utils/hash_map/hash_map.h"

int main() {
  char buffer_in[256] =
      "Conan is a MIT-licensed, Open Source package manager for C and C++ "
      "development for C and C++ development, allowing development teams to "
      "easily and efficiently manage their packages and dependencies across "
      "platforms and build systems.";
  char buffer_out[256] = {0};

  z_stream defstream = {
      .zalloc = Z_NULL,
      .zfree = Z_NULL,
      .opaque = Z_NULL,
      .avail_in = (uInt)strlen(buffer_in),
      .next_in = (Bytef *)buffer_in,
      .avail_out = (uInt)sizeof(buffer_out),
      .next_out = (Bytef *)buffer_out,
  };

  deflateInit(&defstream, Z_BEST_COMPRESSION);
  deflate(&defstream, Z_FINISH);
  deflateEnd(&defstream);

  printf("Uncompressed size is: %lu\n", strlen(buffer_in));
  printf("Compressed size is: %lu\n", strlen(buffer_out));

  printf("ZLIB VERSION: %s\n", zlibVersion());

  return 0;
}

// int main() {
//   printf("=== hello \n");
//   hash_map *m = new_hash_map();
//   printf("=== put start!!! \n");
//   put_to_hash_map(m, "sunjin", "aaaa");
//   put_to_hash_map(m, "aaa", "hogehoge");

//   printf("sunjin value is %s\n", get_value_from_hash_map(m, "sunjin"));

//   int result = http_serve();
//   if (result < 0) {
//     printf("Error: Failed serve\n");
//     return result;
//   }

//   return 0;
// }
