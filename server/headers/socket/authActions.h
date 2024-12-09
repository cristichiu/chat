#ifndef SK_AUTHACTIONS_H
#define SK_AUTHACTIONS_H

#include "index.h"

void handle_whoami(int sd, int *socket);
void handle_create_grup(int sd, int *socket);
void handle_logoff(int sd, int *socket);
void handle_see_my_grups(int sd, int *socket);

#endif