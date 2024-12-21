#include "../index.h"

void handle_login(Client *sd) {
    ALogin login;
    int rs = recv(sd->socket, &login, sizeof(ALogin), 0);
    if(verifyConn(sd, rs)) return;
    UserSessions lgSess = loginUser(login.private_username, login.password, "todo");
    StringRes res;
    if(!lgSess.id) {
        sprintf(res.res, "Ceva nu a mers bine");
        res.status = 404;
        sprintf(res.args, "%s %s", r_print, r_end_wait);
    } else {
        sprintf(res.res, "%ld", lgSess.token);
        res.status = 200;
        sprintf(res.args, "%s %s", r_save_token, r_end_wait);
    }
    send(sd->socket, &res, sizeof(StringRes), 0);
}

void handle_register(Client *sd) {
    ARegister regs;
    int rs = recv(sd->socket, &regs, sizeof(ARegister), 0);
    if(verifyConn(sd, rs)) return;
    StringRes res;
    int status = createUser(regs.private_username, regs.username, regs.password);
    if(status) {
        sprintf(res.res, "User-ul nu a fost creat.");
        res.status = status;
    } else {
        sprintf(res.res, "User-ul a fost creat cu succes.");
        res.status = 200;
    }
    sprintf(res.args, "%s %s", r_print, r_end_wait);
    send(sd->socket, &res, sizeof(StringRes), 0);
}
