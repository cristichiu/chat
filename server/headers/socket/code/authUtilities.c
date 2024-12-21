#include "../index.h"

int verifyConn(Client *sd, int res) {
    if(res <= 0) {
        printf("Client deconectat, socket: %d\n", sd->socket);
        close(sd->socket);
        sd->socket = 0;
        SSL_shutdown(sd->ssl);
        SSL_free(sd->ssl);
        sd->ssl = NULL;
        return 1;
    } else return 0;
}

Users handle_user_session_verify(Client *sd) {
    Users user;
    user.id = 0;
    char buffer[16];
    int res = read(sd->socket, buffer, sizeof(buffer));
    buffer[res] = '\0';
    if(verifyConn(sd, res)) return user;
    UserSessions session = getUserSessionByToken(ldtoa(buffer));
    if(!session.id) return user;
    user = getUserByLInt(session.user_id, US_FOR_ID);
    return user;
}

Grups handle_chat_session_verify(Client *sd) {
    Grups grup;
    grup.id = 0;
    char buffer[16];
    int res = read(sd->socket, buffer, sizeof(buffer));
    buffer[res] = '\0';
    if(verifyConn(sd, res)) return grup;
    grup = getGrupByLId(ldtoa(buffer), GS_FOR_PUBLIC_ID);
    return grup;
}
