#ifndef DB_H_  
#define DB_H_
#include <time.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>

#define c_DB "../DB"

typedef struct Users {
    int id;
    char private_username[64];
    char password[64];
    char username[64];
    short int deleted;
    long long unsigned int secret_key;
    struct tm time_created;
} Users;

typedef struct UserSessions {
    int id;
    char IP[32];
    char token[64];
    short int deleted;
    int user_id;
} UserSessions;

typedef struct Messages {
    int id;
    char message[1024];
    struct tm time_created;
    short int deleted;
    short int dm;
    int user_id;
} Messages;

char *createUser(Users user);

#endif 