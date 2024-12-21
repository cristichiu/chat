#ifndef A_AUTHACTIONS_H
#define A_AUTHACTIONS_H

#include "index.h"

void ma_whoami(int cl, SSL *ssl, int *prs);
void ma_logoff(int cl, SSL *ssl, int *prs);
void ma_create_grup(int cl, SSL *ssl, int *prs);
void ma_see_my_grups(int cl, SSL *ssl, int *prs);
void ma_focus_grup(int cl, SSL *ssl, int *prs);

#endif
