#ifndef SK_AUTHUTILITIES_H
#define SK_AUTHUTILITIES_H

#include "index.h"

Users handle_user_session_verify(Client *sd);
Grups handle_chat_session_verify(Client *sd);
int verifyConn(Client *sd, int res);

#endif
