#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include "./headers/lib.h"
#include "./headers/db/index.h"

#define MAX 80
#define PORT 8080
#define SA struct sockaddr

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

void terminate_connection(int connfd)
{
    write(connfd, "ex", strlen("ex"));
    close(connfd);
    printf("%sConnection terminated.%s\n", CYAN, RESET);
    exit(0);
}

void handle_client(int connfd, int sockfd)
{
    char buff[MAX];
    char success[] = "success\n";
    char register_request[] = "reg_request";
    char login_request[] = "lgn_request";
    char error[] = "error\n";
    int n;
    char username[64];
    char private_username[64];
    char password[64];
    long int tkn = 0;
    for (;;)
    {
        bzero(buff, MAX);
        read(connfd, buff, sizeof(buff));

        if (read <= 0)
        {
            printf("%sClient disconnected or error reading.%s\n", ERROR, RESET);
            break;
        }

        if (strncmp("1", buff, strlen("1")) == 0)
        {
            write(connfd, register_request, strlen(register_request));
        }
        else if (strncmp("2", buff, strlen("2")) == 0)
        {
            write(connfd, login_request, strlen(login_request));
        }
        else if (strncmp("REG:", buff, 4) == 0)
        {
            register_split_string(buff, username, private_username, password);
            if (createUser(private_username, username, password))
            {
                printf("%sUser-ul nu a fost creat.%s\n", ERROR, RESET);
            }
            else
                printf("%sUser creat cu succes%s\n", SUCCESS, RESET);
        }
        else if (strncmp("LGN:", buff, 4) == 0)
        {
            UserSessions login = loginUser(private_username, password, "todo");
            if (!login.id)
            {
                printf("%sCeva nu a mers bine la logare%s\n", ERROR, RESET);
            }
            else
            {
                printf("%sLogat cu succes%s\n", SUCCESS, RESET);
               char token_message[256];  // Make sure it's big enough
                sprintf(token_message, "TKN:%li\n", login.token); // Add the newline
                write(connfd, token_message, strlen(token_message));
            }
        }

        if (strncmp("ex", buff, 2) == 0)
        {
            terminate_connection(connfd);
            break;
        }
    }
}

int main()
{
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("%sSocket creation failed...%s\n", ERROR, RESET);
        exit(0);
    }
    else
        printf("%sSocket successfully created..%s\n", SUCCESS, RESET);
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    if ((bind(sockfd, (SA *)&servaddr, sizeof(servaddr))) != 0)
    {
        printf("%sSocket bind failed...%s\n", ERROR, RESET);
        exit(0);
    }
    else
        printf("%sSocket successfully binded..%s\n", SUCCESS, RESET);

    if ((listen(sockfd, 5)) != 0)
    {
        printf("%sListen failed...%s\n", ERROR, RESET);
        exit(0);
    }
    else
        printf("%sServer listening..%s\n", SUCCESS, RESET);
    len = sizeof(cli);

    connfd = accept(sockfd, (SA *)&cli, &len);
    if (connfd < 0)
    {
        printf("%sServer accept failed...%s\n", ERROR, RESET);
        exit(0);
    }
    else
    {
        printf("%sServer accept the client...%s\n", SUCCESS, RESET);
    }

    handle_client(connfd, sockfd);

    close(sockfd);
}
