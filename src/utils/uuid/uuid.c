#include "uuid.h"

#include <stdlib.h>
#include <uuid/uuid.h>

#define UUID_LENGTH 37
extern const char* new_uuid() {
  uuid_t u4;
  uuid_generate(u4);

  char* out = (char*)malloc(sizeof(char) * UUID_LENGTH);
  uuid_unparse(u4, out);
  return out;
}
