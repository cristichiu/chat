#ifndef DB_H_  
#define DB_H_
#include <time.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include "./lib.h"

#define c_DB "../DB"
#define c_sessions "../DB/sessions.chat"
#define c_users "../DB/users.chat"
#define c_grups "../DB/grups.chat"
#define c_grup_members "../DB/grup_members.chat"

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

int createUser(char *private_username, char *username, char *password);
UserSessions loginUser(char *private_username, char *password, char *IP);
int createGrup(char *name, int user_id);
int addMemberInGrup(long int grup_public_id, int user_id, long int target_public_id, int permissions);

int existByUsername(char *username);
//TODO: User getUserByToken(int token);

// Astea doar iti da versiunea publica a structurii, adica elimina 
Users publicUser(Users user);
UserSessions publicUserSession(UserSessions userS);
Grups publicGrup(Grups grup);

#endif 