#include "../index.h"

void handle_whoami(Client *sd) {
    StringRes res;
    Users user = handle_user_session_verify(sd);
    if(!user.id) {
        sprintf(res.res, "Nu am putut gasi sesiunea ta");
        res.status = 404;
        sprintf(res.args, "%s %s", r_print, r_end_wait);
        send(sd->socket, &res, sizeof(StringRes), 0);
        return;
    }
    sprintf(res.res, "Esti cumva asta tu?:\nuser_public_id: %ld\nusername: %s", user.public_id, user.username);
    res.status = 200;
    sprintf(res.args, "%s %s", r_print, r_end_wait);
    send(sd->socket, &res, sizeof(StringRes), 0);
}

void handle_create_grup(Client *sd) {
    StringRes res;
    Users user = handle_user_session_verify(sd);
    if(!user.id) {
        sprintf(res.res, "Nu am putut gasi sesiunea ta");
        res.status = 404;
        sprintf(res.args, "%s %s", r_print, r_end_wait);
        send(sd->socket, &res, sizeof(StringRes), 0);
        return;
    }
    sprintf(res.res, "Sesiunea merge");
    res.status = 200;
    send(sd->socket, &res, sizeof(StringRes), 0);
    char grupName[64];
    int rs = read(sd->socket, grupName, sizeof(grupName));
    grupName[rs] = '\0';
    if(verifyConn(sd, rs)) return;
    int cStatus = createGrup(grupName, user.id);
    if(cStatus) {
        sprintf(res.res, "Ceva a mers prost");
        res.status = cStatus;
    } else {    
        sprintf(res.res, "Ai creat un grup cu succes");
        res.status = 200;
    }
    sprintf(res.args, "%s %s", r_print, r_end_wait);
    send(sd->socket, &res, sizeof(StringRes), 0);
}

void handle_logoff(Client *sd) {
    char buffer[16];
    int res = read(sd->socket, buffer, sizeof(buffer));
    buffer[res] = '\0';
    if(verifyConn(sd, res)) return;
    FILE *file = fopen(c_sessions, "r+b");
    if(file == NULL) return;
    UserSessions Sbuffer;
    while(fread(&Sbuffer, sizeof(UserSessions), 1, file)) {
        if(ldtoa(buffer) == Sbuffer.token) {
            Sbuffer.deleted = 1;
            fseek(file, -sizeof(UserSessions), SEEK_CUR);
            fwrite(&Sbuffer, sizeof(UserSessions), 1, file);
            break;
        }
    }
    fclose(file);
    StringRes finn;
    finn.status = 200;
    sprintf(finn.args, "%s %s", r_print, r_end_wait);
    strcpy(finn.res, "Log off");
    send(sd->socket, &finn, sizeof(StringRes), 0);
}

void handle_see_my_grups(Client *sd) {
    StringRes res;
    Users user = handle_user_session_verify(sd);
    if(!user.id) {
        sprintf(res.res, "Nu am gasit sesiunea ta!");
        res.status = 404;
        sprintf(res.args, "%s %s", r_print, r_end_wait);
        send(sd->socket, &res, sizeof(StringRes), 0);
        return;
    }
    GrupMembers *allMyGrups = getAllMyGrups(user.id);
    int count = 0;
    while(allMyGrups[count].user_id) {
        Grups grup = getGrupByLId(allMyGrups[count].grup_id, GS_FOR_ID);
        StringRes res;
        sprintf(res.args, "%s", r_print);
        if(grup.id) {
            sprintf(res.res, "%ld - %s - %d (%d%d)", grup.public_id, grup.name, allMyGrups[count].permissions, allMyGrups[count].accept_by_user, grup.owner== user.id);
            res.status = 200;
        } else {
            sprintf(res.res, "Grup not found");
            res.status = 404;
        }
        send(sd->socket, &res, sizeof(StringRes), 0);
        count++;
    }
    StringRes finn;
    finn.status = 303;
    sprintf(finn.args, "%s", r_end_wait);
    strcpy(finn.res, "FINAL");
    send(sd->socket, &finn, sizeof(StringRes), 0);
}
