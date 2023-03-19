#ifndef _HTTP_
#define _HTTP_

#include "../../utils/hash_map/hash_map.h"

typedef int method_t;
typedef int status_code_t;

typedef struct _request_info {
  method_t method;
  char *path;
  hash_map *header_map;
  hash_map *param_map;
  char *body;
} request_info;

typedef struct _response_info {
  char *response_body;
} response_info;

#define METHOD_GET (method_t)1
#define METHOD_POST (method_t)2
#define METHOD_DELETE (method_t)3
#define METHOD_PUT (method_t)4
#define METHOD_OPTIONS (method_t)5

// https://developer.mozilla.org/ja/docs/Web/HTTP/Status
// 必要なら増やす
#define STATUS_CODE_OK (status_code_t)200;
#define STATUS_CODE_CREATED (status_code_t)201;
#define STATUS_CODE_BAD_REQUEST (status_code_t)400;
#define STATUS_CODE_INTERNAL_SERVER_ERROR (status_code_t)500;

extern int http_serve(void);

#endif
