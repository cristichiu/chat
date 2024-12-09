#include "../index.h"

Users handle_session_verify(long int token) {
    UserSessions session = getUserSessionByToken(token);
    Users user;
    user.id = 0;
    if(!session.id) return user;
    user = getUserByLInt(session.user_id, US_FOR_ID);
    return user;
}

void handle_whoami(int sd, int *socket) {
    char buffer[16];
    int res = read(sd, buffer, sizeof(buffer));
    buffer[res] = '\0';
    if(res <= 0) {
        printf("Client deconectat, socket: %d\n", sd);
        close(sd);
        *socket = 0;
    } else {
        StringRes res;
        Users user = handle_session_verify(ldtoa(buffer));
        if(!user.id) {
            sprintf(res.res, "Nu am putut gasi sesiunea ta");
            res.status = 404;
            send(sd, &res, sizeof(StringRes), 0);
            return;
        }
        sprintf(res.res, "Esti cumva asta tu?:\nuser_public_id: %ld\nsession_token: %ld\nusername: %s\n", user.public_id, ldtoa(buffer), user.username);
        res.status = 200;
        send(sd, &res, sizeof(StringRes), 0);
    }
}

void handle_create_grup(int sd, int *socket) {
    char buffer[16];
    int res = read(sd, buffer, sizeof(buffer));
    buffer[res] = '\0';
    if(res <= 0) {
        printf("Client deconectat, socket: %d\n", sd);
        close(sd);
        *socket = 0;
    } else {
        StringRes res;
        Users user = handle_session_verify(ldtoa(buffer));
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
}