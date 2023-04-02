
#include "user.h"
#include <stdlib.h>
#include "../../utils/uuid/uuid.h"
#include <string.h>
#include <stdio.h>

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
        printf("========== u is NULL\n");
        return;
    }
    printf("========== user.id: %s\n", u->id);
    printf("========== user.name: %s\n", u->name);
    printf("========== user.age: %d\n", u->age);
    printf("========== user.email: %s\n", u->email);
    // printf("========== user.id: %s\n", u->id);
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
