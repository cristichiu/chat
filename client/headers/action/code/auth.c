#include "../auth.h"

void ma_login(int cl, int *prc) {
    ALogin user;
    printf("%sprivate_username: %s", AUTH, RESET); scanf("%s", user.private_username);
    printf("%spassword: %s", AUTH, RESET); scanf("%s", user.password);
    send(cl, &user, sizeof(ALogin), 0);
    return;
}

void ma_register(int cl, int *prc) {
    ARegister user;
    printf("%susername: %s", AUTH, RESET); scanf("%s", user.username);
    printf("%sprivate_username: %s", AUTH, RESET); scanf("%s", user.private_username);
    printf("%spassword: %s", AUTH, RESET); scanf("%s", user.password);
    send(cl, &user, sizeof(ARegister), 0);
    return;
}
