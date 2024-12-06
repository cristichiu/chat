#ifndef DB_MODELS_H
#define DB_MODELS_H

#include  "libraries.h"

typedef struct Users {
    long int id;
    long int public_id;
    char private_username[64];
    char password[64];
    char username[64];
    short int deleted;
    struct tm time_created;
} Users;

typedef struct UserSessions {
    long int id;
    long int user_id;
    long int token;
    char IP[32];
    short int deleted;
    struct tm time_created;
} UserSessions;

typedef enum perm { p_init=11111, p_write=10000,p_read=1000,p_invite=100,p_kick=10,p_gPerm=1 } perm; // formula = perm%(read*10)/read == 2 for acces, == 1 for denied

typedef struct GrupMembers {
    long int user_id;
    long int grup_id;
    short int accept_by_user;
    short int deleted;
    int permissions; // write|read|invite|kick|give permissions -> 5 decimals -> 1 for denied, 2 for access (ex: 12111)
} GrupMembers;

typedef struct Grups {
    long int id;
    long int public_id;
    long int owner;
    char name[64];
    short int deleted;
    struct tm time_created;
} Grups;

typedef struct Messages {
    long int id;
    long int public_id;
    long int grup_id;
    long int user_id;
    char message[1024];
    short int deleted;
    struct tm time_created;
} Messages;


#endif