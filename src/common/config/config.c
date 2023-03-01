#include "config.h"

#include <stdio.h>
#include <stdlib.h>

const int DEFAULT_PORT = 8088;

extern config setup() {
  int port = DEFAULT_PORT;
  char *port_str = getenv("PORT");
  if (port_str != NULL) {
    port = atoi(port_str);
  }

  config c = {.port = port};
  return c;
}
