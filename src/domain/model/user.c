
#include "user.h"

#include <stdlib.h>
#include <string.h>

#include "../../common/logger/logger.h"
#include "../../utils/uuid/uuid.h"

extern user *new_user(const char *name, int age, const char *email, const char *password) {
  user *u = malloc(sizeof(user));
  u->id = (char *)new_uuid();
  u->name = strdup(name);
  u->age = age;
  u->email = strdup(email);
  u->password = strdup(password);
  return u;
}

extern user *user_of(const char *id, const char *name, int age, const char *email, const char *password) {
  user *u = malloc(sizeof(user));
  u->id = strdup(id);
  u->name = strdup(name);
  u->age = age;
  u->email = strdup(email);
  u->password = strdup(password);
  return u;
}

extern void debug_user_print(user *u) {
  if (u == NULL) {
    DEBUG_PRINTLN("user is NULL");
    return;
  }
  DEBUG_PRINT("user.id: %s\n", u->id);
  DEBUG_PRINT("user.name: %s\n", u->name);
  DEBUG_PRINT("user.age: %d\n", u->age);
  DEBUG_PRINT("user.email: %s\n", u->email);
  DEBUG_PRINT("user.id: %s\n", u->id);
}

extern void free_user(user *u) {
  free(u->id);
  u->id = NULL;
  free(u->name);
  u->name = NULL;
  u->age = 0;
  free(u->email);
  u->email = NULL;
  free(u->password);
  u->password = NULL;
  free(u);
  u = NULL;
}
