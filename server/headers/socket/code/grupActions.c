#include "../index.h"

void handle_see_focus_grup(Client *sd) {
    Users user = handle_user_session_verify(sd);
    Grups grup = handle_chat_session_verify(sd);
    StringRes res;
    if(!user.id || !grup.id) {
        sprintf(res.res, "Am pierdut sesiunea cu grupul sau cu userul");
        res.status = 404;
        sprintf(res.args, "%s %s", r_print, r_end_wait);
        SSL_write(sd->ssl, &res, sizeof(StringRes));
        return;
    }
    GrupMembers member = getGrupMember(user.id, grup.id);
    sprintf(res.res, "%ld - %s - %d (%d%d)\n", grup.public_id, grup.name, member.permissions, member.accept_by_user, grup.owner == user.id);
    res.status = 200;
    sprintf(res.args, "%s %s", r_print, r_end_wait);
    SSL_write(sd->ssl, &res, sizeof(StringRes));
}

void handle_add_new_meber(Client *sd) {
    Users user = handle_user_session_verify(sd);
    Grups grup = handle_chat_session_verify(sd);
    StringRes res;
    if(!user.id || !grup.id) {
        sprintf(res.res, "Am pierdut sesiunea cu grupul sau cu userul");
        res.status = 404;
        sprintf(res.args, "%s %s", r_print, r_end_wait);
        SSL_write(sd->ssl, &res, sizeof(StringRes));
        return;
    }
    int permissions = p_init+p_read+p_write;
    char sTarget[16];
    int rs = read(sd->socket, sTarget, sizeof(sTarget));
    sTarget[rs] = '\0';
    int status = addMemberInGrup(grup.public_id, user.id, ldtoa(sTarget), permissions);
    if(verifyConn(sd, rs)) return;
    if(status) {
        sprintf(res.res, "Ceva nu a mers cum trebuie");
        res.status = status;

    } else {
        sprintf(res.res, "User adaugat cu succes");
        res.status = 200;
    }
    sprintf(res.args, "%s %s", r_print, r_end_wait);
    SSL_write(sd->ssl, &res, sizeof(StringRes));
}

void handle_see_grup_members(Client *sd) {
    Users user = handle_user_session_verify(sd);
    Grups grup = handle_chat_session_verify(sd);
    StringRes res;
    strcpy(res.res, "");
    if(!user.id || !grup.id) {
        sprintf(res.res, "Am pierdut sesiunea cu grupul sau cu userul");
        res.status = 404;
        sprintf(res.args, "%s %s", r_print, r_end_wait);
        SSL_write(sd->ssl, &res, sizeof(StringRes));
        return;
    }
    GrupMembers *allMembers = getAllGrupMembers(grup.id);
    int count = 0;
    while(allMembers[count].user_id) {
        Users grupUser = getUserByLInt(allMembers[count].user_id, US_FOR_ID);
        if(user.id) {
            int current_length = strlen(res.res);
            int remaining_space = MAX_LENGTH_IN_RES - current_length;
            int needed_space = snprintf(NULL, 0, "%ld - %s - %d (%d)\n", grupUser.public_id, grupUser.username, allMembers[count].permissions, allMembers[count].accept_by_user);
            if (needed_space + 1 <= remaining_space) {
                snprintf(res.res + current_length, remaining_space, "%ld - %s - %d (%d)\n", grupUser.public_id, grupUser.username, allMembers[count].permissions, allMembers[count].accept_by_user);
            }
        }
        count++;
    }
    res.status = 200;
    sprintf(res.args, "%s %s", r_print, r_end_wait);
    SSL_write(sd->ssl, &res, sizeof(StringRes));
}

void handle_write_message(Client *sd) {
    Users user = handle_user_session_verify(sd);
    Grups grup = handle_chat_session_verify(sd);
    StringRes res;
    strcpy(res.res, "");
    if(!user.id || !grup.id) {
        sprintf(res.res, "Am pierdut sesiunea cu grupul sau cu userul");
        res.status = 404;
        sprintf(res.args, "%s %s", r_print, r_end_wait);
        SSL_write(sd->ssl, &res, sizeof(StringRes));
        return;
    }
    char message[1024];
    int rs = read(sd->socket, message, sizeof(message));
    message[rs] = '\0';
    if(message[strlen(message)-1] == '\n') message[strlen(message)-1] = '\0';
    int status = createMessage(message, grup.id, user.id);
    sprintf(res.args, "%s %s", r_print, r_end_wait);
    if(status) {
        sprintf(res.res, "Nu am reusit sa scriu mesajul");
        res.status = status;
        SSL_write(sd->ssl, &res, sizeof(StringRes));
    } else {
        sprintf(res.res, "Ai scris un mesaj cu succes");
        res.status = 200;
        SSL_write(sd->ssl, &res, sizeof(StringRes));
    }
}

void handle_accept_grup_inv(Client *sd) {
    Users user = handle_user_session_verify(sd);
    Grups grup = handle_chat_session_verify(sd);
    StringRes res;
    strcpy(res.res, "");
    if(!user.id || !grup.id) {
        sprintf(res.res, "Am pierdut sesiunea cu grupul sau cu userul");
        res.status = 404;
        sprintf(res.args, "%s %s", r_print, r_end_wait);
        SSL_write(sd->ssl, &res, sizeof(StringRes));
        return;
    }
    int status = acceptInvitation(user.id, grup.id);
    if(status) {
        sprintf(res.res, "Ceva nu a mers bine");
        res.status = status;
    } else {
        sprintf(res.res, "Ai acceptat invitatia cu succes");
        res.status = 200;
    }
    sprintf(res.args, "%s %s", r_print, r_end_wait);
    SSL_write(sd->ssl, &res, sizeof(StringRes));
}

void handle_see_grup_messages(Client *sd) {
    Users user = handle_user_session_verify(sd);
    Grups grup = handle_chat_session_verify(sd);
    StringRes res;
    strcpy(res.res, "");
    if(!user.id || !grup.id) {
        sprintf(res.res, "Am pierdut sesiunea cu grupul sau cu userul");
        res.status = 404;
        sprintf(res.args, "%s %s", r_print, r_end_wait);
        SSL_write(sd->ssl, &res, sizeof(StringRes));
        return;
    }
    GrupMembers verify = getGrupMember(user.id, grup.id);
    if(!verify.user_id || verify.permissions%(p_read*10)/p_read == 1 || !verify.accept_by_user || verify.deleted) {
        sprintf(res.res, "Nu ai acces sa vezi ce este in acest grup");
        res.status = 403;
        sprintf(res.args, "%s %s", r_print, r_end_wait);
        SSL_write(sd->ssl, &res, sizeof(StringRes));
        return;
    }
    Messages *msgs = getMessagesByLInt(grup.id, MSGS_FOR_GRUP_ID);
    int count = 0;
    while(msgs[count].id) {
        Users user = getUserByLInt(msgs[count].user_id, US_FOR_ID);
        if(user.id) {
            int current_length = strlen(res.res);
            int remaining_space = MAX_LENGTH_IN_RES - current_length;
            int needed_space = snprintf(NULL, 0, "%s: %d-%02d-%02d | %02d:%02d:%02d -> %s\n", user.username, msgs[count].time_created.tm_year + 1900, msgs[count].time_created.tm_mon + 1, msgs[count].time_created.tm_mday, msgs[count].time_created.tm_hour, msgs[count].time_created.tm_min, msgs[count].time_created.tm_sec, msgs[count].message);
            if (needed_space + 1 <= remaining_space) {
                snprintf(res.res + current_length, remaining_space, "%s: %d-%02d-%02d | %02d:%02d:%02d -> %s\n", user.username, msgs[count].time_created.tm_year + 1900, msgs[count].time_created.tm_mon + 1, msgs[count].time_created.tm_mday, msgs[count].time_created.tm_hour, msgs[count].time_created.tm_min, msgs[count].time_created.tm_sec, msgs[count].message);
            }
        }
        count++;
    }
    sprintf(res.args, "%s %s", r_print, r_end_wait);
    res.status = 200;
    SSL_write(sd->ssl, &res, sizeof(StringRes));
}
