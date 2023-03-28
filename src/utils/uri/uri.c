#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef URI_CONVERT_BUFFER_SIZE
#define URI_CONVERT_BUFFER_SIZE 1024
#endif

// https://gist.github.com/sasairc/20ffb55dbe225dbd6a8258f7a2f92a38

extern int decode_uri(char* const src, char** dest) {
  size_t buffer_size = URI_CONVERT_BUFFER_SIZE;
  size_t current_position = 0;

  *dest = (char*)malloc(sizeof(char) * URI_CONVERT_BUFFER_SIZE);
  if (*dest == NULL) {
    printf("uri.decode_uri failed allocate memory\n");
    return -1;
  }
  memset(*dest, '\0', URI_CONVERT_BUFFER_SIZE);

  char* p = src;
  for (;;) {
    if (*p == '\0') {
      break;
    }

    if ((current_position + 2) >= buffer_size) {
      buffer_size += URI_CONVERT_BUFFER_SIZE;
      *dest = (char*)realloc(*dest, sizeof(char) * buffer_size);
      if (*dest == NULL) {
        printf("uri.decode_uri failed reallocate memory\n");
        return -1;
      }
      memset(*dest + current_position, '\0', URI_CONVERT_BUFFER_SIZE);
    }

    if (*p == '%') {
      p++;

      //   TODO warningが出ないようにする
      sscanf(p, "%2X", *(dest) + current_position);
      p += 2;
      current_position++;
      continue;
    }

    *(*(dest) + current_position) = *p;
    p++;
    current_position++;
  }
  return 0;
}

extern int encode_uri(char* const src, char** dest) {
  size_t buffer_size = URI_CONVERT_BUFFER_SIZE;
  size_t current_position = 0;

  *dest = (char*)malloc(sizeof(char) * URI_CONVERT_BUFFER_SIZE);
  if (*dest == NULL) {
    printf("uri.encode_uri failed allocate memory\n");
    return -1;
  }
  memset(*dest, '\0', URI_CONVERT_BUFFER_SIZE);

  char* p = src;
  for (;;) {
    if (*p == '\0') {
      break;
    }
    if ((current_position + 3) >= buffer_size) {
      buffer_size += URI_CONVERT_BUFFER_SIZE;
      *dest = (char*)realloc(*dest, sizeof(char) * buffer_size);
      if (*dest == NULL) {
        printf("uri.encode_uri failed reallocate memory\n");
        return -1;
      }
      memset(*dest + current_position, '\0', URI_CONVERT_BUFFER_SIZE);
    }

    if ((*p >= 0x20 && *p <= 0x39) ||  // [0-9]
        (*p >= 0x41 && *p <= 0x5A) ||  // [A-Z]
        (*p >= 0x61 && *p <= 0x7A) ||  // [a-z]
        (*p == 0x2E) ||                // .
        (*p == 0x2F) ||                // /
        (*p == 0x3A)) {
      *(*(dest) + current_position) = *p;
      current_position++;
      p++;
      continue;
    }

    /*
     * アドレス p が指す領域の値を、二桁の16進数へ変換
     * 途中、暗黙のキャストにより 4byte へ拡張され出力されてしまうので、
     * ビットへのAND操作により、下位 8bit (1byte) ぶんを取り出す
     */
    current_position +=
        sprintf(*dest + current_position, "%%%02X", *p & 0x000000FF);
    p++;
  }

  return 0;
}
