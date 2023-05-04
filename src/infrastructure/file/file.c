#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFF_SIZE 1024

extern char *get_file_content(const char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("failed open file: %s\n", filename);
    return NULL;
  }

  if (fseek(fp, 0, SEEK_END) != 0) {
    printf("failed seek file: %s\n", filename);
    return NULL;
  }

  long size = ftell(fp);
  rewind(fp);

  char *content = (char *)malloc(size + 1);
  int fread_result = fread(content, size, 1, fp);
  if (fread_result == 0) {
    int feof_result = feof(fp);
    if (feof_result != 0) {
      printf("failed fread file: %s\n", filename);
      return NULL;
    }
  }

  content[size] = '\0';

  fclose(fp);

  return content;
}
