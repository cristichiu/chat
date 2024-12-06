#ifndef DB_USER_H
#define DB_USER_H

#include "index.h"

typedef enum IntUserSearch { US_FOR_ID, US_FOR_PUBLIC_ID } IntUserSearch;
typedef enum StringUserSearch { US_FOR_PRIVATE_USERNAME, US_FOR_USERNAME, US_FOR_PASSWORD } StringUserSearch;

int createUser(char *private_username, char *username, char *password);
UserSessions loginUser(char *private_username, char *password, char *IP);

Users getUserByLInt(long int search, IntUserSearch searchFor);
Users getUserByString(char *search, StringUserSearch searchFor);
UserSessions getUserSessionByToken(long int search);

#endif