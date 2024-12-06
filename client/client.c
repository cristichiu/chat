#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>
#include "./headers/auth.h"
#include "./headers/client_utils.h"

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

long long token = 0;

void handle_client(int sockfd, int connfd)
{
    char buff[MAX];
    int n;
    int wait_response = 0;
    for (;;)
    {
        bzero(buff, sizeof(buff));

        if (wait_response == 0)
        {
            printf("%sEnter  : %s", USER, RESET);
            n = 0;
            while ((buff[n++] = getchar()) != '\n');
            buff[n - 1] = '\0'; 
            if (strcmp(buff, "1") == 0)
            {
                handle_registration_request(sockfd);
            }
            if (strcmp(buff, "2") == 0)
            {
                handle_login_request(sockfd);
            }
            write(sockfd, buff, sizeof(buff));
            bzero(buff, sizeof(buff));
        }

        read(sockfd, buff, sizeof(buff));

        if (!(strlen(buff) >= 4 && isupper(buff[0]) && isupper(buff[1]) && isupper(buff[2]) && buff[3] == ':'))
        {
            printf("%sServer : %s%s%s\n", CYAN, WHITE, buff, RESET);
        }

        if ((strncmp(buff, "TKN", 3)) == 0)
        {
            token = atoll(buff + 4);
            wait_response = 0;
            printf("%s You're logged in! \n%s", SUCCESS, RESET);
        }
        else if ((strncmp(buff, "ex", 2)) == 0)
        {
            printf("%sClient Exit...\n%s", ERROR, RESET);
            close(sockfd);
            exit(0);
            break;
        }
    }
}

int main()
{
        srand(time(NULL));
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("%ssocket creation failed...\n%s", ERROR, RESET);
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    char ip_address[16];
    snprintf(ip_address, sizeof(ip_address), "127.0.0.%d", 100 + rand() % 101);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) != 0)
    {
        printf("%sconnection with the server failed...\n%s", ERROR, RESET);
        perror("connect");
        exit(0);
    }
    else
    {
        printf("%sconnected to the server..\n%s", SUCCESS, RESET);
        printf("%sChoose an option:\n1. Register\n2. Login\n%s", USER, RESET);
    }

    handle_client(sockfd, connfd);

    close(sockfd);
}