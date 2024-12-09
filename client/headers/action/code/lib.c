#include "../lib.h"

long int ldtoa(char *str) {
    long int res = 0;
    for(int i=0; i<strlen(str); i++) {
        res = res*10+(str[i]-'0');
    }
    return res;
}

int verifyConnection(int rs, int cl) {
    if(rs <= 0) {
        printf("Client deconectat, socket: %d\n", cl);
        close(cl);
        return 1;
    }
    return 0;
}

char *getSessionToken() {
    FILE *file = fopen(DB_sessions, "rb");
    if(file == NULL) return "0";
    long int token;
    char *cToken = (char*)malloc(16*sizeof(char));
    fread(&token, sizeof(long int), 1, file);
    sprintf(cToken, "%ld", token);
    fclose(file);
    return cToken;
}

char *getChatSessionToken() {
    FILE *file = fopen(DB_chatSession, "rb");
    if(file == NULL) return "0";
    long int token;
    char *cToken = (char*)malloc(16*sizeof(char));
    fread(&token, sizeof(long int), 1, file);
    sprintf(cToken, "%ld", token);
    fclose(file);
    return cToken;
}