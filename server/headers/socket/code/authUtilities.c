#include "../index.h"

Users handle_user_session_verify(int sd, int *socket) {
    Users user;
    user.id = 0;
    char buffer[16];
    int res = read(sd, buffer, sizeof(buffer));
    buffer[res] = '\0';
    if(res <= 0) {
        printf("Client deconectat, socket: %d\n", sd);
        close(sd);
        *socket = 0;
        return user;
    }
    UserSessions session = getUserSessionByToken(ldtoa(buffer));
    if(!session.id) return user;
    user = getUserByLInt(session.user_id, US_FOR_ID);
    return user;
}

Grups handle_chat_session_verify(int sd, int *socket) {
    Grups grup;
    grup.id = 0;
    char buffer[16];
    int res = read(sd, buffer, sizeof(buffer));
    buffer[res] = '\0';
    if(res <= 0) {
        printf("Client deconectat, socket: %d\n", sd);
        close(sd);
        *socket = 0;
        return grup;
    }
    grup = getGrupByLId(ldtoa(buffer), GS_FOR_PUBLIC_ID);
    return grup;
}