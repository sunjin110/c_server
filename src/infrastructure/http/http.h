#ifndef _HTTP_
#define _HTTP_

#include "../../utils/hash_map/hash_map.h"

typedef int method_t;

typedef struct _request_info {
  method_t method;
  char *path;
  hash_map *header_map;
  hash_map *param_map;
} request_info;

extern int http_serve(void);

#endif
