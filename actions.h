#ifndef ACTIONS_H
#define ACTIONS_H

#define a_login "login"
#define a_register "register"
#define a_whoami "whoami"
#define a_create_grup "crtGrup"

typedef struct ARegister {
    char username[64];
    char private_username[64];
    char password[64];
} ARegister;

typedef struct ALogin {
    char private_username[64];
    char password[64];
} ALogin;

typedef struct StringRes {
    int status;
    char res[1024];
} StringRes;

#endif