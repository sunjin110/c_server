#include <stdio.h>

#include "src/common/logger/logger.h"
#include "src/infrastructure/http/http.h"

int main() {
  // // test
  // mysql_config config = {
  //   .database = "test",
  //   .host = "localhost",
  //   .pass = "dummy_pass",
  //   .user = "dummy_user",
  // };
  // mysql_setup(&config);

  // TODO 環境変数からmodeを切り替えするようにする
  set_log_mode(LOG_MODE_DEBUG);

  int result = http_serve();
  if (result < 0) {
    printf("Error: Failed serve\n");
    return result;
  }
  return 0;
}
