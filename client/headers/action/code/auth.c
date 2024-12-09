#include "../auth.h"

#define ERROR "\033[0;31m"
#define SUCCESS "\033[0;32m"
#define USER "\033[0;33m"
#define BLUE "\033[0;34m"
#define AUTH "\033[0;35m"
#define CYAN "\033[0;36m"
#define WHITE "\033[0;37m"
#define RESET "\033[0m"

void ma_login(int cl) {
    ALogin user;
    printf("%sprivate_username: %s", AUTH, RESET); scanf("%s", user.private_username);
    printf("%spassword: %s", AUTH, RESET); scanf("%s", user.password);
    send(cl, &user, sizeof(ALogin), 0);
    StringRes res;
    verifyConnection(recv(cl, &res, sizeof(StringRes), 0), cl);
    if(res.status == 200) {
        FILE *newSess = fopen(DB_sessions, "wb");
        if(newSess == NULL) return;
        long int iToken = ldtoa(res.res);
        fwrite(&iToken, sizeof(long int), 1, newSess);
        fclose(newSess);
    } else printf("%s%d - %s%s\n", ERROR, res.status, res.res, RESET);
    return;
}

void ma_register(int cl) {
    ARegister user;
    printf("%susername: %s", AUTH, RESET); scanf("%s", user.username);
    printf("%sprivate_username: %s", AUTH, RESET); scanf("%s", user.private_username);
    printf("%spassword: %s", AUTH, RESET); scanf("%s", user.password);
    send(cl, &user, sizeof(ARegister), 0);
    StringRes res;
    verifyConnection(recv(cl, &res, sizeof(StringRes), 0), cl);
    if(res.status == 200) printf("%s%d - %s%s\n", SUCCESS, res.status, res.res, RESET); else printf("%s%d - %s%s\n", ERROR, res.status, res.res, RESET);
    return;
}

void ma_whoami(int cl) {
    FILE *file = fopen(DB_sessions, "rb");
    StringRes res;
    if(file == NULL) {
        send(cl, "0", 16, 0);
        verifyConnection(recv(cl, &res, sizeof(StringRes), 0), cl);
        printf("%d\n%s", res.status, res.res);
    } else {
        long int token;
        char cToken[16];
        fread(&token, sizeof(long int), 1, file);
        sprintf(cToken, "%ld", token);
        send(cl, cToken, 16, 0);
        fclose(file);
        verifyConnection(recv(cl, &res, sizeof(StringRes), 0), cl);
        printf("%d\n%s", res.status, res.res);
    }
}

void ma_logoff(int cl) {
    FILE *file = fopen(DB_sessions, "wb");
    if(file == NULL) return;
    long int reset = 0;
    fwrite(&reset, sizeof(long int), 1, file);
    fclose(file);
    return;
}

void ma_create_grup(int cl) {
    FILE *file = fopen(DB_sessions, "rb");
    StringRes res;
    if(file == NULL) {
        send(cl, "0", 16, 0);
    } else {
        long int token;
        char cToken[16];
        fread(&token, sizeof(long int), 1, file);
        sprintf(cToken, "%ld", token);
        send(cl, cToken, 16, 0);
        fclose(file);
    }
    verifyConnection(recv(cl, &res, sizeof(StringRes), 0), cl);
    if(res.status == 200) {
        char grupName[64];
        printf("Grup name: "); scanf("%s", grupName);
        send(cl, grupName, sizeof(grupName), 0);
        verifyConnection(recv(cl, &res, sizeof(StringRes), 0), cl);
        printf("%d - %s\n", res.status, res.res);
    } else {
        printf("%d - %s\n", res.status, res.res);
    }
}