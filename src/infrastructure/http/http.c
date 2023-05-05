#include "http.h"

#include <errno.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "../../common/converter/converter.h"
#include "../../common/logger/logger.h"
#include "../../utils/equal_str/equal_str.h"
#include "../../utils/hash_map/hash_map.h"
#include "../../utils/linked_str_list/linked_str_list.h"
#include "../../utils/num/num.h"
#include "../../utils/split/split.h"
#include "../../utils/str/str.h"
#include "../../utils/trim/trim.h"
#include "../../utils/uri/uri.h"
#include "../mysql/mysql.h"
#include "router.h"
#include "thread_pool.h"

#define REQUEST_SIZE 2048
#define REQUEST_CHUNK_SIZE 1024
#define THREAD_POOL_SIZE 100

static const int MAX_PENDING_CONNECTION = 10;
static const int PORT = 8089;

static const char *METHOD_GET_VALUE = "GET";
static const char *METHOD_POST_VALUE = "POST";
static const char *METHOD_DELETE_VALUE = "DELETE";
static const char *METHOD_PUT_VALUE = "PUT";
static const char *METHOD_OPTIONS_VALUE = "OPTIONS";

static char *get_request_str(int client_sock);
static request_info *get_request(int client_sock);
static hash_map *get_header_map(const char *request);
static method_t convert_method(const char *method_str);
static hash_map *get_param_map(const char *param_str);
static void free_request(request_info *info);
static char *get_body_str(const char *request_str, size_t content_length);
static void handle_client(int client_socket);

extern int http_serve() {
  INFO_PRINTLN("start server: http://localhost:%d", PORT);

  // test
  mysql_config config = {
      .database = "test",
      .host = "localhost",
      .pass = "dummy_pass",
      .user = "dummy_user",
  };
  mysql_setup(&config);

  // read socket
  // ipv4 protocol,
  int server_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (server_sock < 0) {
    ERR_PRINTLN("Error: failed make server_sock");
    return -1;
  }

  struct sockaddr_in addr = {
      .sin_family = AF_INET,
      .sin_port = htons(PORT),
      .sin_addr =
          {
              .s_addr = INADDR_ANY,
          },
  };

  int server_result = bind(server_sock, (struct sockaddr *)&addr, sizeof(addr));
  if (server_result < 0) {
    ERR_PRINTLN("Error: failed bind socket");
    return -1;
  }

  // listen
  DEBUG_PRINT("=== start listen...\n");
  int listen_result = listen(server_sock, MAX_PENDING_CONNECTION);
  if (listen_result < 0) {
    ERR_PRINT("Error: failed listen\n");
    switch (errno) {
      case EADDRINUSE:
        ERR_PRINT("reason: Address already in use\n");
        break;
      case EBADF:
        ERR_PRINT("reason: Bad file descriptor\n");
        break;
      case ENOTSOCK:
        ERR_PRINT("reason: Socket operation on non-socket\n");
        break;
      case EOPNOTSUPP:
        ERR_PRINT("reason: Operation not supported on socket");
        break;
      default:
        ERR_PRINT("reason: unknown, errno:%d\n", errno);
    }

    return -1;
  }

  // make thread_pool
  thread_pool_t pool;
  thread_pool_init(&pool, THREAD_POOL_SIZE);

  for (;;) {
    // accept TCP connection
    DEBUG_PRINT("=== start accept...\n");
    struct sockaddr_in client = {};
    socklen_t client_length = sizeof(client);
    int client_sock = accept(server_sock, (struct sockaddr *)&client, &client_length);
    if (client_sock < 0) {
      ERR_PRINT("Error: failed accept.\n");
      switch (errno) {
        case EAGAIN:
          ERR_PRINT("reason: Resource temporarily unavailable\n");
          break;
        case EBADF:
          ERR_PRINT("reason: Bad file descriptor\n");
          break;
        case ECONNABORTED:
          ERR_PRINT("reason: Software caused connection abort\n");
          break;
        case EFAULT:
          ERR_PRINT("reason: Bad address\n");
          break;
        case EINTR:
          ERR_PRINT("reason: Interrupted system call\n");
          break;
        default:
          ERR_PRINT("reason: errno:%d\n", errno);
          break;
      }

      return -1;
    }

    thread_pool_submit(&pool, (void (*)(void *))handle_client, INT_TO_VOIDP(client_sock));
  }

  thread_pool_wait(&pool);
  thread_pool_destroy(&pool);

  int close_result = close(server_sock);
  if (close_result < 0) {
    WARN_PRINT("Warning: failed close server_sock\n");
    WARN_PRINT("reason: errno:%d\n", errno);
  }

  return 0;
}

static void handle_client(int client_sock) {
  request_info *request = get_request(client_sock);

  DEBUG_PRINT("=== start routing\n");
  char *response_body = routing(request);

  size_t response_body_size = strlen(response_body);

  size_t response_base_size = strlen("HTTP/1.1 200 OK\nContent-Length: \n\n\n");
  size_t response_content_length_digit = num_digit(response_body_size);

  DEBUG_PRINT("response_body_size is %zd\n", response_body_size);
  DEBUG_PRINT("response_base_size is %zd\n", response_base_size);
  DEBUG_PRINT("response_content_length_digit is %zd\n", response_content_length_digit);

  char response[response_body_size + response_base_size + response_content_length_digit + 1];
  sprintf(response, "HTTP/1.1 200 OK\nContent-Length: %zu\n\n%s\n", response_body_size, response_body);
  DEBUG_PRINT("response is %s\n", response);

  DEBUG_PRINT("=========== response_body_free\n");
  if (response_body != NULL) {
    free(response_body);
  }

  DEBUG_PRINT("=== start write...\n");
  int write_result = write(client_sock, response, strlen(response) + 1);
  if (write_result < 0) {
    ERR_PRINT("Error: failed write to write_sock\n");
    ERR_PRINT("reason: errno:%d\n", errno);
    // return -1;
    // TODO free_request
    // free_request(request);
    // return;
  }
  DEBUG_PRINT("=== finished write...\n");

  // free
  DEBUG_PRINT("=== free start ===\n");
  free_request(request);

  // close
  int close_result = close(client_sock);
  if (close_result < 0) {
    WARN_PRINT("Warning: failed close write_sock\n");
    WARN_PRINT("reason: errno:%d\n", errno);
  }
}

static request_info *get_request(int client_sock) {
  char *request_str = get_request_str(client_sock);
  if (request_str == NULL) {
    return NULL;
  }

  // get method and path
  char *methodAndPath = get_before_str(request_str, "\r\n");
  linked_str_list *method_and_path_list = split(methodAndPath, " ");
  char *method_str = value_linked_str_list(method_and_path_list, 0);

  // paramはない場合がある
  char *path_and_params = value_linked_str_list(method_and_path_list, 1);

  linked_str_list *path_and_params_list = split(path_and_params, "?");

  // param
  DEBUG_PRINT("=== get param\n");
  hash_map *param_map = NULL;
  if (path_and_params_list->size >= 2) {
    char *params_str = value_linked_str_list(path_and_params_list, 1);
    char *decoded_params_str = NULL;
    int decode_result = decode_uri(params_str, &decoded_params_str);
    if (decode_result != 0) {
      ERR_PRINTLN("failed decode uri. params_str:%s", params_str);
      free(params_str);
      params_str = NULL;
      decoded_params_str = NULL;
      return NULL;
    }

    param_map = get_param_map(decoded_params_str);
    free(params_str);
    free(decoded_params_str);
    params_str = NULL;
    decoded_params_str = NULL;
  }

  // header_strを取得する
  DEBUG_PRINT("=== get header\n");
  hash_map *header_map = get_header_map(request_str);

  // body
  DEBUG_PRINT("=== get body\n");
  char *content_length_str = get_value_from_hash_map(header_map, "Content-Length");

  char *body_str = NULL;
  if (content_length_str != NULL) {
    int content_length = atoi(content_length_str);
    body_str = get_body_str(request_str, content_length);
  }

  request_info *info = (request_info *)malloc(sizeof(request_info));
  info->method = convert_method(method_str);
  info->path = value_linked_str_list(path_and_params_list, 0);
  info->param_map = param_map;
  info->header_map = header_map;
  info->body = body_str;

  free(method_str);
  method_str = NULL;
  free(methodAndPath);
  methodAndPath = NULL;
  free(path_and_params);
  path_and_params = NULL;
  free(request_str);
  request_str = NULL;
  free_linked_str_list(method_and_path_list);
  method_and_path_list = NULL;
  free_linked_str_list(path_and_params_list);
  path_and_params_list = NULL;

  return info;
}

static hash_map *get_param_map(const char *params_str) {
  if (params_str == NULL) {
    return NULL;
  }

  linked_str_list *params_str_list = split(params_str, "&");
  if (params_str_list == NULL) {
    ERR_PRINT("failed split params_str: [%s]\n", params_str);
    return NULL;
  }

  linked_str_element *current_params = params_str_list->first_ptr;
  hash_map *param_map = new_hash_map();
  for (;;) {
    if (current_params == NULL) {
      break;
    }

    linked_str_list *key_and_value = split(current_params->value, "=");

    put_to_hash_map(param_map, key_and_value->first_ptr->value, key_and_value->last_ptr->value);
    free_linked_str_list(key_and_value);
    current_params = current_params->next_ptr;
  }
  current_params = NULL;

  free_linked_str_list(params_str_list);

  return param_map;
}

static void free_request(request_info *info) {
  if (info == NULL) {
    return;
  }

  info->method = 0;
  free(info->path);
  info->path = NULL;

  DEBUG_PRINT("=== start header_map free\n");
  if (info->header_map != NULL) {
    free_hash_map(info->header_map);
  }

  DEBUG_PRINT("=== start param_map free\n");
  if (info->param_map != NULL) {
    free_hash_map(info->param_map);
  }

  DEBUG_PRINT("=== start body free\n");
  if (info->body != NULL) {
    DEBUG_PRINT("body is %s\n", info->body);
    free(info->body);
    // info->body = NULL;
  }

  info->header_map = NULL;
  info->param_map = NULL;
  free(info);
}

static char *get_request_str(int client_sock) {
  ssize_t request_length = 0;
  ssize_t read_bytes = 0;
  char *request = NULL;
  for (;;) {
    char buf[REQUEST_CHUNK_SIZE];
    read_bytes = read(client_sock, buf, REQUEST_CHUNK_SIZE);
    if (read_bytes < 0) {
      ERR_PRINT("Error: failed read...\n");
      ERR_PRINT("reason: errno:%d\n", errno);
      return NULL;
    }

    request = (char *)realloc(request, request_length + read_bytes);
    if (request == NULL) {
      ERR_PRINT("Error: failed realloc request\n");
      ERR_PRINT("reason: errno:%d\n", errno);
      return NULL;
    }
    memcpy(request + request_length, buf, read_bytes);
    request_length += read_bytes;
    if (read_bytes < REQUEST_CHUNK_SIZE) {
      break;
    }
  }
  return request;
}

static method_t convert_method(const char *method_str) {
  if (method_str == NULL) {
    ERR_PRINT("failed convert method, method_str is NULL\n");
    return -1;
  }

  if (strcmp(method_str, METHOD_GET_VALUE) == 0) {
    return METHOD_GET;
  }

  if (strcmp(method_str, METHOD_POST_VALUE) == 0) {
    return METHOD_POST;
  }

  if (strcmp(method_str, METHOD_DELETE_VALUE) == 0) {
    return METHOD_DELETE;
  }

  if (strcmp(method_str, METHOD_PUT_VALUE) == 0) {
    return METHOD_PUT;
  }

  if (strcmp(method_str, METHOD_OPTIONS_VALUE) == 0) {
    return METHOD_OPTIONS;
  }

  ERR_PRINT("failed convert method, method_str is %s\n", method_str);
  return -1;
}

static char *get_body_str(const char *request, size_t content_length) {
  if (content_length <= 0) {
    DEBUG_PRINT("not found content length");
    return NULL;
  }

  char *header_end = (char *)strstr(request, "\r\n\r\n");
  if (header_end == NULL) {
    ERR_PRINT("not found header\n");
    return NULL;
  }

  char *body_str = (char *)malloc(content_length + 1);
  strncpy(body_str, header_end + 4, content_length);
  body_str[content_length] = '\0';
  return body_str;
}

static hash_map *get_header_map(const char *request) {
  char *header_end = (char *)strstr(request, "\r\n\r\n");
  if (header_end == NULL) {
    ERR_PRINT("Error: not found header\n");
    return NULL;
  }
  int header_length = header_end - request + 4;
  char header[header_length + 1];
  strncpy(header, request, header_length);
  header[header_length] = '\0';

  linked_str_list *header_line_list = split(header, "\r\n");
  hash_map *header_map = new_hash_map();

  // 1個目はpathなのでスルー
  linked_str_element *current_request_str_line = header_line_list->first_ptr->next_ptr;
  for (;;) {
    if (current_request_str_line == NULL) {
      break;
    }

    char *key_end = (char *)strstr(current_request_str_line->value, ":");
    if (key_end == NULL) {
      ERR_PRINT("not found key end\n");
      break;
    }
    size_t key_size = key_end - current_request_str_line->value;
    char *key = (char *)malloc(key_size + 1);
    strncpy(key, current_request_str_line->value, key_size);
    key[key_size] = '\0';

    size_t value_size = strlen(current_request_str_line->value) - key_size - 1;  // 「:」の文の長さを削除
    char *value = (char *)malloc(value_size + 1);
    strncpy(value, current_request_str_line->value + key_size + 1,
            value_size);  // 「:」の長さをskip
    value[value_size] = '\0';

    put_to_hash_map(header_map, key, trim(value, NULL));
    current_request_str_line = current_request_str_line->next_ptr;
    free(key);
    free(value);
  }

  free_linked_str_list(header_line_list);
  return header_map;
}
