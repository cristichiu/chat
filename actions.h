#ifndef ACTIONS_H
#define ACTIONS_H

#define a_login "login"
#define a_register "register"
#define a_whoami "whoami"
#define a_create_grup "crtGrup"
#define a_logoff "logOff"
#define a_see_my_grups "sMyGrups"
#define a_see_focus_grup "sFocGrup"
#define a_add_new_member "aNeMem"

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