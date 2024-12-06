#include "server_utils.h"
#include "./lib.h"
#include "./db/index.h"
#define ERROR "\033[0;31m"
#define SUCCESS "\033[0;32m"
#define USER "\033[0;33m"
#define BLUE "\033[0;34m"
#define AUTH "\033[0;35m"
#define CYAN "\033[0;36m"
#define WHITE "\033[0;37m"
#define RESET "\033[0m"


void register_split_string(const char *input, char *username, char *private, char *password)
{
    const char *processed_input = input + 4;
    if (strlen(input) < 4)
    {
        username[0] = '\0';
        private[0] = '\0';
        password[0] = '\0';
        return;
    }

    char buffer[64];
    strncpy(buffer, processed_input, sizeof(buffer));
    buffer[sizeof(buffer) - 1] = '\0';

    char *token = strtok(buffer, "#>!");

    if (token != NULL)
    {
        strncpy(username, token, 64);
        username[63] = '\0';
        token = strtok(NULL, "#>!");
    }

    if (token != NULL)
    {
        strncpy(private, token, 64);
        private[63] = '\0';
        token = strtok(NULL, "#>!");
    }

    if (token != NULL)
    {
        strncpy(password, token, 64);
        password[63] = '\0';
    }
}


void login_split_string(const char *input, char *private_username, char *password) {
     const char *processed_input = input + 4;
    if (strlen(input) < 4)
    {
        private_username[0] = '\0';
        password[0] = '\0';
        return;
    }

    char buffer[64];
    strncpy(buffer, processed_input, sizeof(buffer));
    buffer[sizeof(buffer) - 1] = '\0';

    char *token = strtok(buffer, "#>!");

    if (token != NULL)
    {
        strncpy(private_username, token, 64);
        private_username[63] = '\0';
        token = strtok(NULL, "#>!");
    }

    if (token != NULL)
    {
        strncpy(password, token, 64);
        password[63] = '\0';
    }
}

void terminate_connection(int connfd)
{
    write(connfd, "ex", strlen("ex"));
    close(connfd);
    printf("%sConnection terminated.%s\n", CYAN, RESET);
    exit(0);
}

void handle_registration(int connfd, char *buffer) {
    char username[64], private_username[64], password[64];
    register_split_string(buffer, username, private_username, password);
    if (createUser(private_username, username, password)) {
        printf("%sUser creation failed.%s\n", ERROR, RESET);
         write(connfd, "error\n", strlen("error\n")); 
    } else {
       write(connfd, "User created successfully.\n", strlen("User created successfully.\n")); 
        printf("%sUser created successfully.%s\n", SUCCESS, RESET);
    }
}


void handle_login(int connfd, char *buffer) {
    char private_username[64], password[64];
    login_split_string(buffer, private_username, password); 

    UserSessions login = loginUser(private_username, password, "todo");
    if (!login.id) {
        printf("%sLogin failed.%s\n", ERROR, RESET);
        write(connfd, "error\n", strlen("error\n")); 
    } else {
        printf("%sSuccessful login.%s\n", SUCCESS, RESET);
        char token_message[256];
        sprintf(token_message, "TKN:%li\n", login.token);
        write(connfd, token_message, strlen(token_message));
    }
}