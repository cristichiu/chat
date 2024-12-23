#include "../grupActions.h"

void ma_grup_deselect(int cl, SSL *ssl, int *prc) {
    FILE *file = fopen(DB_chatSession, "wb");
    if(file == NULL) return;
    long int reset = 0;
    fwrite(&reset, sizeof(long int), 1, file);
    fclose(file);
    *prc = 0;
}

void ma_see_focus_grup(int cl, SSL *ssl, int *prc) {
    char *token = getSessionToken();
    char *chatToken = getChatSessionToken();
    SSL_write(ssl, token, 16);
    SSL_write(ssl, chatToken, 16);
}

void ma_add_new_member(int cl, SSL *ssl, int *prc) {
    char *token = getSessionToken();
    char *chatToken = getChatSessionToken();
    SSL_write(ssl, token, 16);
    SSL_write(ssl, chatToken, 16);
    long int target;
    char cTarget[16];
    printf("Target public ID: "); scanf("%ld", &target);
    sprintf(cTarget, "%ld", target);
    SSL_write(ssl, cTarget, 16);
}

void ma_see_all_grup_members(int cl, SSL *ssl, int *prc) {
    char *token = getSessionToken();
    char *chatToken = getChatSessionToken();
    SSL_write(ssl, token, 16);
    SSL_write(ssl, chatToken, 16);
}

void ma_write_message(int cl, SSL *ssl, int *prc) {
    char *token = getSessionToken();
    char *chatToken = getChatSessionToken();
    SSL_write(ssl, token, 16);
    SSL_write(ssl, chatToken, 16);
    char message[1024];
    fgets(message, 1024, stdin);
    SSL_write(ssl, message, sizeof(message));
}

void ma_accept_grup_inv(int cl, SSL *ssl, int *prc) {
    char *token = getSessionToken();
    char *chatToken = getChatSessionToken();
    SSL_write(ssl, token, 16);
    SSL_write(ssl, chatToken, 16);
}

void ma_see_all_grup_messages(int cl, SSL *ssl, int *prc) {
    char *token = getSessionToken();
    char *chatToken = getChatSessionToken();
    SSL_write(ssl, token, 16);
    SSL_write(ssl, chatToken, 16);
    *prc = 2;
}
