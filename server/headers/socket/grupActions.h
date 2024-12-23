#ifndef SK_GRUPACTIONS_H
#define SK_GRUPACTIONS_H

#include "index.h"

void handle_see_focus_grup(Client *sd);
void handle_add_new_meber(Client *sd);
void handle_see_grup_members(Client *sd);
void handle_write_message(Client *sd, Client sds[MAX_CLIENTS]);
void handle_accept_grup_inv(Client *sd);
void handle_see_grup_messages(Client *sd);

#endif
