#include "../authActions.h"

void ma_whoami(int cl, SSL *ssl, int *prc) {
    char *token = getSessionToken();
    StringRes res;
    SSL_write(ssl, token, 16);
    return;
}

void ma_logoff(int cl, SSL *ssl, int *prc) {
    char *token = getSessionToken();
    SSL_write(ssl, token, 16);
    FILE *file = fopen(DB_sessions, "wb");
    if(file == NULL) return;
    long int reset = 0;
    fwrite(&reset, sizeof(long int), 1, file);
    fclose(file);
    return;
}

void ma_create_grup(int cl, SSL *ssl, int *prc) {
    char *token = getSessionToken();
    StringRes res;
    SSL_write(ssl, token, 16);
    if(verifyConnection(SSL_read(ssl, &res, sizeof(StringRes)), cl, ssl)) return;
    if(res.status == 200) {
        char grupName[64];
        printf("Grup name: "); scanf("%s", grupName);
        SSL_write(ssl, grupName, sizeof(grupName));
    }
    return;
}

void ma_see_my_grups(int cl, SSL *ssl, int *prc) {
    char *token = getSessionToken();
    StringRes res;
    SSL_write(ssl, token, 16);
    return;
}

void ma_focus_grup(int cl, SSL *ssl, int *prc) {
    long int chatSession;
    printf("Chat public ID: "); scanf("%ld", &chatSession);
    FILE *file = fopen(DB_chatSession, "wb");
    if(file == NULL) return;
    fwrite(&chatSession, sizeof(long int), 1, file);
    fclose(file);
    *prc = 0;
    return;
}
