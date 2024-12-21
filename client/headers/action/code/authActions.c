#include "../authActions.h"

void ma_whoami(int cl, int *prc) {
    char *token = getSessionToken();
    StringRes res;
    send(cl, token, 16, 0);
    return;
}

void ma_logoff(int cl, int *prc) {
    char *token = getSessionToken();
    send(cl, token, 16, 0);
    FILE *file = fopen(DB_sessions, "wb");
    if(file == NULL) return;
    long int reset = 0;
    fwrite(&reset, sizeof(long int), 1, file);
    fclose(file);
    return;
}

void ma_create_grup(int cl, int *prc) {
    char *token = getSessionToken();
    StringRes res;
    send(cl, token, 16, 0);
    if(verifyConnection(recv(cl, &res, sizeof(StringRes), 0), cl)) return;
    if(res.status == 200) {
        char grupName[64];
        printf("Grup name: "); scanf("%s", grupName);
        send(cl, grupName, sizeof(grupName), 0);
    }
    return;
}

void ma_see_my_grups(int cl, int *prc) {
    char *token = getSessionToken();
    StringRes res;
    send(cl, token, 16, 0);
    return;
}

void ma_focus_grup(int cl, int *prc) {
    long int chatSession;
    printf("Chat public ID: "); scanf("%ld", &chatSession);
    FILE *file = fopen(DB_chatSession, "wb");
    if(file == NULL) return;
    fwrite(&chatSession, sizeof(long int), 1, file);
    fclose(file);
    *prc = 0;
    return;
}
