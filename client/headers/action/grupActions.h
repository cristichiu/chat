#ifndef A_GRUPACTIONS_H
#define A_GRUPACTIONS_H

#include "index.h"

void ma_grup_deselect(int cl, SSL *ssl, int *prc);
void ma_see_focus_grup(int cl, SSL *ssl, int *prc);
void ma_add_new_member(int cl, SSL *ssl, int *prc);
void ma_see_all_grup_members(int cl, SSL *ssl, int *prc);
void ma_write_message(int cl, SSL *ssl, int *prc);
void ma_accept_grup_inv(int cl, SSL *ssl, int *prc);
void ma_see_all_grup_messages(int cl, SSL *ssl, int *prc);

#endif
