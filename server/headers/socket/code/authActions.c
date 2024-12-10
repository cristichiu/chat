#include "../index.h"

void handle_whoami(int sd, int *socket) {
    StringRes res;
    Users user = handle_user_session_verify(sd, socket);
    if(!user.id) {
        sprintf(res.res, "Nu am putut gasi sesiunea ta");
        res.status = 404;
        send(sd, &res, sizeof(StringRes), 0);
        return;
    }
    sprintf(res.res, "Esti cumva asta tu?:\nuser_public_id: %ld\nusername: %s\n", user.public_id, user.username);
    res.status = 200;
    send(sd, &res, sizeof(StringRes), 0);
}

void handle_create_grup(int sd, int *socket) {
    StringRes res;
    Users user = handle_user_session_verify(sd, socket);
    if(!user.id) {
        sprintf(res.res, "Nu am putut gasi sesiunea ta");
        res.status = 404;
        send(sd, &res, sizeof(StringRes), 0);
        return;
    }
    sprintf(res.res, "Sesiunea merge");
    res.status = 200;
    send(sd, &res, sizeof(StringRes), 0);
    char grupName[64];
    int rs = read(sd, grupName, sizeof(grupName));
    grupName[rs] = '\0';
    if(rs <= 0) {
        printf("Client deconectat, socket: %d\n", sd);
        close(sd);
        *socket = 0;
        return;
    }
    int cStatus = createGrup(grupName, user.id);
    if(cStatus) {
        sprintf(res.res, "Ceva a mers prost");
        res.status = cStatus;
    } else {    
        sprintf(res.res, "Ai creat un grup cu succes");
        res.status = 200;
    }
    send(sd, &res, sizeof(StringRes), 0);
}

void handle_logoff(int sd, int *socket) {
    char buffer[16];
    int res = read(sd, buffer, sizeof(buffer));
    buffer[res] = '\0';
    if(res <= 0) {
        printf("Client deconectat, socket: %d\n", sd);
        close(sd);
        *socket = 0;
        return;
    }
    FILE *file = fopen(c_sessions, "r+b");
    if(file == NULL) return;
    UserSessions Sbuffer;
    while(fread(&Sbuffer, sizeof(UserSessions), 1, file)) {
        if(ldtoa(buffer) == Sbuffer.token) {
            Sbuffer.deleted = 1;
            fseek(file, -sizeof(UserSessions), SEEK_CUR);
            fwrite(&Sbuffer, sizeof(UserSessions), 1, file);
            fclose(file);
            return;
        }
    }
    fclose(file);
}

void handle_see_my_grups(int sd, int *socket) {
    StringRes res;
    Users user = handle_user_session_verify(sd, socket);
    if(!user.id) {
        sprintf(res.res, "Nu am gasit sesiunea ta!");
        res.status = 404;
        send(sd, &res, sizeof(StringRes), 0);
        return;
    }
    GrupMembers *allMyGrups = getAllMyGrups(user.id);
    int count = 0;
    strcpy(res.res, "");
    while(allMyGrups[count].user_id) {
        Grups grup = getGrupByLId(allMyGrups[count].grup_id, GS_FOR_ID);
        if(grup.id) {
            int current_length = strlen(res.res);
            int remaining_space = MAX_LENGTH_IN_RES - current_length;
            int needed_space = snprintf(NULL, 0, "%ld - %s - %d (%d%d)\n", grup.public_id, grup.name, allMyGrups[count].permissions, allMyGrups[count].accept_by_user, grup.owner == user.id);
            if (needed_space + 1 <= remaining_space) {
                snprintf(res.res + current_length, remaining_space, "%ld - %s - %d (%d%d)\n", grup.public_id, grup.name, allMyGrups[count].permissions, allMyGrups[count].accept_by_user, grup.owner == user.id);
            }
        }
        count++;
    }
    res.status = 200;
    send(sd, &res, sizeof(StringRes), 0);
}