#include "../grupActions.h"

void ma_grup_deselect(int cl, int *prc) {
    FILE *file = fopen(DB_chatSession, "wb");
    if(file == NULL) return;
    long int reset = 0;
    fwrite(&reset, sizeof(long int), 1, file);
    fclose(file);
    *prc = 0;
}

void ma_see_focus_grup(int cl, int *prc) {
    char *token = getSessionToken();
    char *chatToken = getChatSessionToken();
    send(cl, token, 16, 0);
    send(cl, chatToken, 16, 0);
}

void ma_add_new_member(int cl, int *prc) {
    char *token = getSessionToken();
    char *chatToken = getChatSessionToken();
    send(cl, token, 16, 0);
    send(cl, chatToken, 16, 0);
    long int target;
    char cTarget[16];
    printf("Target public ID: "); scanf("%ld", &target);
    sprintf(cTarget, "%ld", target);
    send(cl, cTarget, 16, 0);
}

void ma_see_all_grup_members(int cl, int *prc) {
    char *token = getSessionToken();
    char *chatToken = getChatSessionToken();
    send(cl, token, 16, 0);
    send(cl, chatToken, 16, 0);
}

void ma_write_message(int cl, int *prc) {
    char *token = getSessionToken();
    char *chatToken = getChatSessionToken();
    send(cl, token, 16, 0);
    send(cl, chatToken, 16, 0);
    printf("Scrie mesajul tau: ");
    char message[1024];
    getchar();
    fgets(message, 1024, stdin);
    send(cl, message, sizeof(message), 0);
}

void ma_accept_grup_inv(int cl, int *prc) {
    char *token = getSessionToken();
    char *chatToken = getChatSessionToken();
    send(cl, token, 16, 0);
    send(cl, chatToken, 16, 0);
}

void ma_see_all_grup_messages(int cl, int *prc) {
    char *token = getSessionToken();
    char *chatToken = getChatSessionToken();
    send(cl, token, 16, 0);
    send(cl, chatToken, 16, 0);
}
