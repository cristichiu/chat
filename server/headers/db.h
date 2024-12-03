#ifndef DB_H_  
#define DB_H_
#include <time.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include "./lib.h"

#define c_DB "../DB"

typedef struct Users {
    int id;
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

typedef struct Messages {
    int id;
    char message[1024];
    short int deleted;
    short int dm;
    int user_id;
    struct tm time_created;
} Messages;

int createUser(Users user);
UserSessions loginUser(int user, char *IP);

#endif 