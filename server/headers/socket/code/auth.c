#include "../index.h"

void handle_login(int sd, int *socket) {
    ALogin login;
    int res = recv(sd, &login, sizeof(ALogin), 0);
    if(res <= 0) {
        printf("Client deconectat, socket: %d\n", sd);
        close(sd);
        *socket = 0;
    } else {
        UserSessions lgSess = loginUser(login.private_username, login.password, "todo");
        StringRes res;
        if(!lgSess.id) {
            sprintf(res.res, "Ceva nu a mers bine");
            res.status = 404;
        } else {
            sprintf(res.res, "%ld", lgSess.token);
            res.status = 200;
        }
        send(sd, &res, sizeof(StringRes), 0);
    }
}

void handle_register(int sd, int *socket) {
    ARegister regs;
    int res = recv(sd, &regs, sizeof(ARegister), 0);
    if(res <= 0) {
        printf("Client deconectat, socket: %d\n", sd);
        close(sd);
        *socket = 0;
    } else {
        StringRes res;
        int status = createUser(regs.private_username, regs.username, regs.password);
        if(status) {
            sprintf(res.res, "User-ul nu a fost creat.");
            res.status = status;
        } else {
            sprintf(res.res, "User-ul a fost creat cu succes.");
            res.status = 200;
        }
        send(sd, &res, sizeof(StringRes), 0);
    }
}