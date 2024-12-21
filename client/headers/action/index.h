#ifndef A_INDEX_H
#define A_INDEX_H

#include <openssl/ssl.h>
#include <openssl/err.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "auth.h"
#include "authActions.h"
#include "grupActions.h"
#include "lib.h"
#include "../../../actions.h"

#define DB_sessions "../DB/session.chat"
#define DB_chatSession "../DB/chatSession.chat"

typedef struct Menu {
    char *action;
    char *instruction;
    void (*handler)(int, SSL *, int *);
} Menu;

typedef struct Res {
    char *response;
    void (*handler)(StringRes res);
} Res;

#define ERROR "\033[0;31m"
#define SUCCESS "\033[0;32m"
#define USER "\033[0;33m"
#define BLUE "\033[0;34m"
#define AUTH "\033[0;35m"
#define CYAN "\033[0;36m"
#define WHITE "\033[0;37m"
#define RESET "\033[0m"

#endif
