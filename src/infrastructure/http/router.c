#include <jansson.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../../lib/mustach/mustach-jansson.h"
#include "../../common/logger/logger.h"
#include "../../utils/hash_map/hash_map.h"
#include "../file/file.h"
#include "http.h"
#include "template/hello.html.h"
#include "template/index.html.h"

static char *top(request_info *info);
static char *hello(request_info *info);

extern char *routing(request_info *info) {
  method_t method = info->method;
  char *path = info->path;

  DEBUG_PRINT("path:%s method:%d\n", path, method);

  if (strcmp(path, "/") == 0) {
    switch (method) {
      case METHOD_GET:
        return top(info);
    }
  }

  if (strcmp(path, "/hello") == 0) {
    switch (method) {
      case METHOD_GET:
        return hello(info);
    }
  }

  INFO_PRINT("not found path: %s\n", path);

  // no content
  return strdup("");
}

static char *top(request_info *info) {
  char *response = strdup((const char *)index_html);
  return response;
}

static char *hello(request_info *info) {
  char *name = get_value_from_hash_map(info->param_map, "name");
  if (name == NULL) {
    name = strdup("");
  }
  json_t *context = json_pack("{s:s}", "name", name);

  char *result = NULL;
  size_t result_size = 0;
  int result_status = mustach_jansson_mem((const char *)hello_html, hello_html_len, context, 0, &result, &result_size);

  if (result_status < 0) {
    ERR_PRINT("failed template hello\n");
    json_decref(context);
    return strdup("");
  }
  json_decref(context);
  return result;
}
