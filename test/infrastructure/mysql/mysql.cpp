#include "mysql.h"

#include <cstdlib>
#include <string>
#include <iostream>
#include <string.h>

using std::getenv;
using std::string;
using std::cout; 
using std::endl;

static int convert_port(char *port_str) {
  if (port_str == NULL) {
    return 13306;
  }
  auto port = atoi(port_str);
  if (port <= 0) {
    return 13306;
  }
  return port;
}

extern mysql_config *test_mysql_config() {

  cout << "======== test mysql config =========" << endl;
  const char *database = getenv("C_SERVER_TEST_MYSQL_DATABASE");
  if (database == NULL) {
    database = "c_server";
  }

  const char *user = getenv("C_SERVER_TEST_MYSQL_USER");
  if (user == NULL) {
    user = "c_server_user";
  }

  const char *password = getenv("C_SERVER_TEST_MYSQL_PASSWORD");
  if (password == NULL) {
    password = "passw0rd";
  }

  const char *host = getenv("C_SERVER_TEST_MYSQL_HOST");
  if (host == NULL) {
    host = "127.0.0.1";
  }

  auto port_str = getenv("C_SERVER_TEST_MYSQL_PORT");
  auto port = convert_port(port_str);
  
  mysql_config *cfg = (mysql_config *)malloc(sizeof(mysql_config));
  cfg->database = strdup(database);
  cfg->user = strdup(user);
  cfg->pass = strdup(password);
  cfg->host = strdup(host);
  cfg->port = port;
  return cfg;
}
