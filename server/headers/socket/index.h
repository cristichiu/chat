#ifndef SK_INDEX_H
#define SK_INDEX_H

#include <openssl/ssl.h>
#include <openssl/err.h>

typedef struct Client {
    int socket;
    SSL *ssl;
} Client;

#include "../db/index.h"
#include "../../../actions.h"

#include "auth.h"
#include "authActions.h"
#include "grupActions.h"
#include "authUtilities.h"

#include "../lib.h"

#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <signal.h>
#include <errno.h>
#include <time.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>

typedef struct Command {
    const char *command;
    void (*handler)(Client *);
} Command;

#endif
