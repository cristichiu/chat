#include "../auth.h"

void ma_login(int cl, SSL *ssl, int *prc) {
    ALogin user;
    printf("%sprivate_username: %s", AUTH, RESET); scanf("%s", user.private_username);
    printf("%spassword: %s", AUTH, RESET); scanf("%s", user.password);
    SSL_write(ssl, &user, sizeof(ALogin));
    return;
}

void ma_register(int cl, SSL *ssl, int *prc) {
    ARegister user;
    printf("%susername: %s", AUTH, RESET); scanf("%s", user.username);
    printf("%sprivate_username: %s", AUTH, RESET); scanf("%s", user.private_username);
    printf("%spassword: %s", AUTH, RESET); scanf("%s", user.password);
    SSL_write(ssl, &user, sizeof(ARegister));
    return;
}
