#ifndef SK_AUTH_H
#define SK_AUTH_H

#include "index.h"

void handle_login(int sd, int *socket);
void handle_register(int sd, int *socket);

#endif