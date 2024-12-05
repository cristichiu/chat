#ifndef DB_USER_H
#define DB_USER_H

#include "index.h"

typedef enum IntUserSearch { S_FOR_ID, S_FOR_PUBLIC_ID } IntUserSearch;
typedef enum StringUserSearch { S_FOR_PRIVATE_USERNAME, S_FOR_USERNAME, S_FOR_PASSWORD } StringUserSearch;

int createUser(char *private_username, char *username, char *password);
UserSessions loginUser(char *private_username, char *password, char *IP);

Users getUserByInt(int search, IntUserSearch searchFor);
Users getUserByString(char *search, StringUserSearch searchFor);
UserSessions getUserSessionByToken(long int search);

#endif