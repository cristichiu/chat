#ifndef SK_GRUPACTIONS_H
#define SK_GRUPACTIONS_H

#include "index.h"

void handle_see_focus_grup(int sd, int *socket);
void handle_add_new_meber(int sd, int *socket);
void handle_see_grup_members(int sd, int *socket);
void handle_write_message(int sd, int *socket);
void handle_accept_grup_inv(int sd, int *socket);
void handle_see_grup_messages(int sd, int *socket);

#endif