#ifndef DOMAIN_USER
#define DOMAIN_USER

typedef struct {
  char *id;
  char *name;
  int age;
  char *email;
  char *password;
} user;

extern user *new_user(const char *name, int age, const char *email,
                      const char *password);
extern user *user_of(const char *id, const char *name, int age,
                     const char *email, const char *password);
extern void free_user(user *u);
extern void debug_user_print(user *u);

#endif
