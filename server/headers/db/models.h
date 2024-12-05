#ifndef DB_MODELS_H
#define DB_MODELS_H

#include  "libraries.h"

typedef struct Users {
    int id;
    int public_id;
    char private_username[64];
    char password[64];
    char username[64];
    short int deleted;
    struct tm time_created;
} Users;

typedef struct UserSessions {
    int id;
    char IP[32];
    long int token;
    short int deleted;
    int user_id;
    struct tm time_created;
} UserSessions;

typedef enum perm { init=11111, write=10000,read=1000,invite=100,kick=10,gPerm=1 } perm; // formula = perm%(read*10)/read == 2 for acces, == 1 for denied

typedef struct GrupMembers {
    int user_id;
    int grup_id;
    short int accept_by_user;
    int deleted;
    int permissions; // write|read|invite|kick|give permissions -> 5 decimals -> 1 for denied, 2 for access (ex: 12111)
} GrupMembers;

typedef struct Grups {
    int id;
    long int public_id;
    char name[64];
    int owner;
    short int deleted;
    struct tm time_created;
} Grups;

typedef struct Messages {
    int id;
    char message[1024];
    short int deleted;
    int user_id;
    int grup_id;
    struct tm time_created;
} Messages;


#endif