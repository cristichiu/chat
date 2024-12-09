#include "../index.h"

void handle_see_focus_grup(int sd, int *socket) {
    Users user = handle_user_session_verify(sd, socket);
    Grups grup = handle_chat_session_verify(sd, socket);
    StringRes res;
    if(!user.id || !grup.id) {
        sprintf(res.res, "Am pierdut sesiunea cu grupul sau cu userul");
        res.status = 404;
        send(sd, &res, sizeof(StringRes), 0);
        return;
    }
    GrupMembers member = getGrupMember(user.id, grup.id);
    sprintf(res.res, "%ld - %s - %d (%d%d)\n", grup.public_id, grup.name, member.permissions, member.accept_by_user, grup.owner == user.id);
    res.status = 200;
    send(sd, &res, sizeof(StringRes), 0);
}

void handle_add_new_meber(int sd, int *socket) {
    Users user = handle_user_session_verify(sd, socket);
    Grups grup = handle_chat_session_verify(sd, socket);
    StringRes res;
    if(!user.id || !grup.id) {
        sprintf(res.res, "Am pierdut sesiunea cu grupul sau cu userul");
        res.status = 404;
        send(sd, &res, sizeof(StringRes), 0);
        return;
    }
    int permissions = p_init+p_read+p_write;
    char sTarget[16];
    int rs = read(sd, sTarget, sizeof(sTarget));
    sTarget[rs] = '\0';
    if(rs <= 0) {
        printf("Client deconectat, socket: %d\n", sd);
        close(sd);
        *socket = 0;
        return;
    }
    int status = addMemberInGrup(grup.public_id, user.id, ldtoa(sTarget), permissions);
    if(status) {
        sprintf(res.res, "Ceva nu a mers cum trebuie");
        res.status = status;

    } else {
        sprintf(res.res, "User adaugat cu succes");
        res.status = 200;
    }
    send(sd, &res, sizeof(StringRes), 0);
}