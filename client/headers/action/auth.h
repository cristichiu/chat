#ifndef A_AUTH_H
#define A_AUTH_H

#include "index.h"

void ma_login(int cl, SSL *ssl, int *prs);
void ma_register(int cl, SSL *ssl, int *prs);

#endif
