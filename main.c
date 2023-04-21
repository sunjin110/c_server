#include <stdio.h>

#include "src/infrastructure/http/http.h"
// #include "src/infrastructure/mysql/mysql.h"

int main() {
  // // test
  // mysql_config config = {
  //   .database = "test",
  //   .host = "localhost",
  //   .pass = "dummy_pass",
  //   .user = "dummy_user",
  // };
  // mysql_setup(&config);

  int result = http_serve();
  if (result < 0) {
    printf("Error: Failed serve\n");
    return result;
  }
  return 0;
}
