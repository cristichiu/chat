#include "../auth.h"

void ma_login(int cl) {
    ALogin user;
    printf("%sprivate_username: %s", AUTH, RESET); scanf("%s", user.private_username);
    printf("%spassword: %s", AUTH, RESET); scanf("%s", user.password);
    send(cl, &user, sizeof(ALogin), 0);
    StringRes res;
    if(verifyConnection(recv(cl, &res, sizeof(StringRes), 0), cl)) return;
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
    if(verifyConnection(recv(cl, &res, sizeof(StringRes), 0), cl)) return;
    if(res.status == 200) printf("%s%d - %s%s\n", SUCCESS, res.status, res.res, RESET); else printf("%s%d - %s%s\n", ERROR, res.status, res.res, RESET);
    return;
}