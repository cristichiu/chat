#include "auth.h"

#include <stdio.h>
#define MAX 100

#define ERROR "\033[0;31m"
#define SUCCESS "\033[0;32m"
#define USER "\033[0;33m"
#define BLUE "\033[0;34m"
#define AUTH "\033[0;35m"
#define CYAN "\033[0;36m"
#define WHITE "\033[0;37m"
#define RESET "\033[0m"
void handle_registration_request(int sockfd)
{
    printf("\n%s######  Registration requested by server  ######%s\n", CYAN, RESET);

    char username[MAX], private_username[MAX], password[MAX];

    printf("%sEnter Username: %s", AUTH, RESET);
    bzero(username, sizeof(username));
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0';

    printf("%sEnter Private Username: %s", AUTH, RESET);
    bzero(private_username, sizeof(private_username));
    fgets(private_username, sizeof(private_username), stdin);
    private_username[strcspn(private_username, "\n")] = '\0';

    printf("%sEnter Password: %s", AUTH, RESET);
    bzero(password, sizeof(password));
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0';

    char reg_data[MAX];
    // >!# used for delimiting the string
    snprintf(reg_data, sizeof(reg_data), "REG:%s>!#%s>!#%s", username, private_username, password);
    write(sockfd, reg_data, sizeof(reg_data));

    printf("%s######  Registration data sent to server  ######%s\n", CYAN, RESET);
}

void handle_login_request(int sockfd)
{
    printf("\n%s######  Login requested by server  ######%s\n", CYAN, RESET);

    char private_username[MAX], password[MAX];

    printf("%sEnter Private Username: %s", AUTH, RESET);
    bzero(private_username, sizeof(private_username));
    fgets(private_username, sizeof(private_username), stdin);
    private_username[strcspn(private_username, "\n")] = '\0';

    printf("%sEnter Password: %s", AUTH, RESET);
    bzero(password, sizeof(password));
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0';

    char reg_data[MAX];
    // >!# used for delimiting the string
    snprintf(reg_data, sizeof(reg_data), "LGN:%s>!#%s", private_username, password);
    write(sockfd, reg_data, sizeof(reg_data));

    printf("%s######  Login data sent to server  ######%s\n", CYAN, RESET);
}
