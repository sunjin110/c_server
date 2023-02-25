#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#define REQUEST_SIZE 2048
#define REQUEST_CHUNK_SIZE 1024

static const int MAX_PENDING_CONNECTION = 10;
static const int PORT = 8088;

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

    // read request
    // char request[REQUEST_SIZE] = {0};
    // ssize_t read_size = read(client_sock, request, REQUEST_SIZE);
    // if (read_size < 0) {
    //     printf("Error: failed read\n");
    //     printf("reason: errno:%d\n", errno);
    //     return -1;
    // }
    // printf("read size is %d\n", REQUEST_SIZE);
    // printf("request is %s\n", request);

    // for (;;) {
    //     char request[256] = {0};
    //     read(client_sock, )
    // }

    ssize_t request_length = 0;
    ssize_t read_bytes = 0;
    char *request = NULL;
    for (;;) {
        char buf[REQUEST_CHUNK_SIZE];
        read_bytes = read(client_sock, buf, REQUEST_CHUNK_SIZE);
        if (read_bytes < 0) {
            printf("Error: failed read...\n");
            printf("reason: errno:%d\n", errno);
            return -1;
        }

        request = realloc(request, request_length + read_bytes);
        if (request == NULL) {
            printf("Error: failed realloc request\n");
            printf("reason: errno:%d\n", errno);
            return -1;
        }
        memcpy(request + request_length, buf, read_bytes);
        request_length += read_bytes;
        if (read_bytes < REQUEST_CHUNK_SIZE) {
            break;
        }
    }

    printf("request is %s\n", request);
    printf("request_length %zd\n", request_length);

    // response message
    char *response = "HTTP/1.1 200 OK\nContent-Length: 12\n\nHello World!\n";
    printf("=== start write...\n");
    int write_result = write(client_sock, response, strlen(response));
    if (write_result < 0) {
      printf("Error: failed write to write_sock\n");
      printf("reason: errno:%d\n", errno);
      return -1;
    }

    // free
    free(request);

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