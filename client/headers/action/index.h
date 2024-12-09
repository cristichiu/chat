#ifndef A_INDEX_H
#define A_INDEX_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "auth.h"
#include "lib.h"
#include "../../../actions.h"

#define DB_sessions "../DB/session.chat"

typedef struct Menu {
    char *action;
    char *instruction;
    void (*handler)(int);
} Menu;

#endif