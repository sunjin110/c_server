#include "http.h"

#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "../../utils/equal_str/equal_str.h"
#include "../../utils/hash_map/hash_map.h"
#include "../../utils/linked_str_list/linked_str_list.h"
#include "../../utils/split/split.h"
#include "../../utils/str/str.h"
#include "../../utils/trim/trim.h"

#define REQUEST_SIZE 2048
#define REQUEST_CHUNK_SIZE 1024

static const int MAX_PENDING_CONNECTION = 10;
static const int PORT = 8089;

const method_t METHOD_GET = 1;
const method_t METHOD_POST = 2;
const method_t METHOD_DELETE = 3;
const method_t METHOD_PUT = 4;
const method_t METHOD_OPTIONS = 5;

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

extern int http_serve() {
  printf("=== start server: http://localhost:%d\n", PORT);

  // read socket
  // ipv4 protocol,
  int server_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (server_sock < 0) {
    printf("Error: failed make server_sock\n");
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
    printf("Error: failed bind socket\n");
    return -1;
  }

  // listen
  printf("=== start listen...\n");
  int listen_result = listen(server_sock, MAX_PENDING_CONNECTION);
  if (listen_result < 0) {
    printf("Error: failed listen\n");
    switch (errno) {
      case EADDRINUSE:
        printf("reason: Address already in use\n");
        break;
      case EBADF:
        printf("reason: Bad file descriptor\n");
        break;
      case ENOTSOCK:
        printf("reason: Socket operation on non-socket\n");
        break;
      case EOPNOTSUPP:
        printf("reason: Operation not supported on socket");
        break;
      default:
        printf("reason: unknown, errno:%d\n", errno);
    }

    return -1;
  }

  for (;;) {
    // accept TCP connection
    printf("=== start accept...\n");
    struct sockaddr_in client = {};
    socklen_t client_length = sizeof(client);
    int client_sock =
        accept(server_sock, (struct sockaddr *)&client, &client_length);
    if (client_sock < 0) {
      printf("Error: failed accept.\n");
      switch (errno) {
        case EAGAIN:
          printf("reason: Resource temporarily unavailable\n");
          break;
        case EBADF:
          printf("reason: Bad file descriptor\n");
          break;
        case ECONNABORTED:
          printf("reason: Software caused connection abort\n");
          break;
        case EFAULT:
          printf("reason: Bad address\n");
          break;
        case EINTR:
          printf("reason: Interrupted system call\n");
          break;
        default:
          printf("reason: errno:%d\n", errno);
          break;
      }

      return -1;
    }

    request_info *request = get_request(client_sock);

    // response message
    char *response = "HTTP/1.1 200 OK\nContent-Length: 12\n\nhello world\n";

    printf("=== start write...\n");
    int write_result = write(client_sock, response, strlen(response) + 1);
    if (write_result < 0) {
      printf("Error: failed write to write_sock\n");
      printf("reason: errno:%d\n", errno);
      return -1;
    }
    printf("=== finished write...\n");

    // free
    printf("=== free start ===\n");
    free_request(request);

    // close
    int close_result = close(client_sock);
    if (close_result < 0) {
      printf("Warning: failed close write_sock\n");
      printf("reason: errno:%d\n", errno);
    }
  }

  int close_result = close(server_sock);
  if (close_result < 0) {
    printf("Warning: failed close server_sock\n");
    printf("reason: errno:%d\n", errno);
  }

  return 0;
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
  printf("=== get param\n");
  hash_map *param_map = NULL;
  if (path_and_params_list->size >= 2) {
    char *params_str = value_linked_str_list(path_and_params_list, 1);
    param_map = get_param_map(params_str);
    free(params_str);
    params_str = NULL;
  }

  // header_strを取得する
  printf("=== get header\n");
  hash_map *header_map = get_header_map(request_str);

  request_info *info = (request_info *)malloc(sizeof(request_info));
  info->method = convert_method(method_str);
  info->path = value_linked_str_list(path_and_params_list, 0);
  info->param_map = param_map;
  info->header_map = header_map;

  // body
  char *content_length_str =
      get_value_from_hash_map(info->header_map, "Content-Length");
  int content_length = atoi(content_length_str);

  char *body_str = get_body_str(request_str, content_length);
  printf("body_str is %s\n", body_str);



  free(body_str);
  body_str = NULL;
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
    printf("failed split params_str: [%s]\n", params_str);
    return NULL;
  }

  linked_str_element *current_params = params_str_list->first_ptr;
  hash_map *param_map = new_hash_map();
  for (;;) {
    if (current_params == NULL) {
      break;
    }

    linked_str_list *key_and_value = split(current_params->value, "=");

    put_to_hash_map(param_map, key_and_value->first_ptr->value,
                    key_and_value->last_ptr->value);
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

  printf("=== start header_map free\n");
  if (info->header_map != NULL) {
    free_hash_map(info->header_map);
  }

  printf("=== start param_map free\n");
  if (info->param_map != NULL) {
    free_hash_map(info->param_map);
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
      printf("Error: failed read...\n");
      printf("reason: errno:%d\n", errno);
      return NULL;
    }

    request = (char *)realloc(request, request_length + read_bytes);
    if (request == NULL) {
      printf("Error: failed realloc request\n");
      printf("reason: errno:%d\n", errno);
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
    printf("failed convert method, method_str is NULL\n");
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

  printf("failed convert method, method_str is %s\n", method_str);
  return -1;
}

static char *get_body_str(const char *request, size_t content_length) {
  if (content_length <= 0) {
    printf("not found content length");
    return NULL;
  }

  char *header_end = (char *)strstr(request, "\r\n\r\n");
  if (header_end == NULL) {
    printf("Error: not found header\n");
    return NULL;
  }

  // size_t header_length = header_end - request + 4;

  char *body_str = (char *)malloc(content_length + 1);
  strncpy(body_str, header_end + 4, content_length);
  body_str[content_length] = '\0';
  // printf("body_str is %s\n", body_str);
  return body_str;
}

static hash_map *get_header_map(const char *request) {
  char *header_end = (char *)strstr(request, "\r\n\r\n");
  if (header_end == NULL) {
    printf("Error: not found header\n");
    return NULL;
  }
  int header_length = header_end - request + 4;
  char header[header_length + 1];
  strncpy(header, request, header_length);
  header[header_length] = '\0';

  linked_str_list *header_line_list = split(header, "\r\n");
  hash_map *header_map = new_hash_map();

  // 1個目はpathなのでスルー
  linked_str_element *current_request_str_line = 
      header_line_list->first_ptr->next_ptr;
  for (;;) {
    if (current_request_str_line == NULL) {
      break;
    }

    char *key_end = (char *)strstr(current_request_str_line->value, ":");
    if (key_end == NULL) {
      printf("not found key end\n");
      break;
    }
    size_t key_size = key_end - current_request_str_line->value;
    char *key = (char *)malloc(key_size + 1);
    strncpy(key, current_request_str_line->value, key_size);
    key[key_size] = '\0';

    size_t value_size = strlen(current_request_str_line->value) - key_size -
                        1;  // 「:」の文の長さを削除
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
