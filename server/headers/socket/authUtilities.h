#ifndef SK_AUTHUTILITIES_H
#define SK_AUTHUTILITIES_H

#include "index.h"

Users handle_user_session_verify(int sd, int *socket);
Grups handle_chat_session_verify(int sd, int *socket);

#endif