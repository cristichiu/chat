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
#define a_see_grup_members "sGrupMe"
#define a_write_message "wrtMsg"
#define a_accept_grup_inv "aGrupIn"
#define a_see_grup_messages "sGrupM"

#define r_save_token "saveToken"
#define r_print "print"
#define r_end_wait "endWait"

#define MAX_LENGTH_IN_RES 1024

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
    char args[MAX_LENGTH_IN_RES];
    char res[MAX_LENGTH_IN_RES];
} StringRes;

#endif
