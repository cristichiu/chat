#ifndef A_LIB_H
#define A_LIB_H

#include "index.h"

long int ldtoa(char *str);
int verifyConnection(int rs, int cl, SSL *ssl);
char *getSessionToken();
char *getChatSessionToken();

#endif
