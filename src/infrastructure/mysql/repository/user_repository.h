#include <mysql.h>
#include <stdio.h>

#include "../../../domain/model/user.h"
#include "../../../repository/user_repository.h"
#include "../mysql.h"

extern user_repository *new_user_repository_for_mysql();
static int insert_user(ctx *c, user *insert_user);
static int delete_user(ctx *c, const char *id);
static user *get_user(ctx *c, const char *id);
