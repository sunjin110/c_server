#ifndef REPOSITORY_USER
#define REPOSITORY_USER

#include "../common/context/context.h"
#include "../domain/model/user.h"

typedef struct _user_repository {
  int (*insert_user)(ctx *c, user *insert_user);
  int (*delete_user)(ctx *c, const char *id);
  user *(*get_user)(ctx *c, const char *id);
} user_repository;

#endif
