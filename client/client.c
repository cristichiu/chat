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

<<<<<<< Updated upstream
void func(int sockfd)
{
    char buff[MAX];
    int n;
    for (;;) {
        bzero(buff, sizeof(buff));
        printf("Enter  : ");
        n = 0;
        while ((buff[n++] = getchar()) != '\n');
        write(sockfd, buff, sizeof(buff));
        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
        printf("Server : %s", buff);

        if ((strncmp(buff, "reg_request", strlen("reg_request"))) == 0) {
            printf("\n######Registration requested by server.######\n");

            
            char username1[MAX], username2[MAX], password[MAX];

            
            printf("Enter Username: ");
            bzero(username1, sizeof(username1));
            fgets(username1, sizeof(username1), stdin);
            username1[strcspn(username1, "\n")] = '\0'; 

            
            printf("Enter Private Username: ");
            bzero(username2, sizeof(username2));
            fgets(username2, sizeof(username2), stdin);
            username2[strcspn(username2, "\n")] = '\0'; 

            
            printf("Enter Password: ");
            bzero(password, sizeof(password));
            fgets(password, sizeof(password), stdin);
            password[strcspn(password, "\n")] = '\0'; 

            
            char reg_data[MAX];
            snprintf(reg_data, sizeof(reg_data), "%s>!#%s>!#%s", username1, username2, password);
            write(sockfd, reg_data, sizeof(reg_data));

            printf("######Registration data sent to server.######\n");
        }


        if ((strncmp(buff, "ex", 2)) == 0) {
            printf("Client Exit...\n");
=======
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
>>>>>>> Stashed changes
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

<<<<<<< Updated upstream
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
=======
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("%ssocket creation failed...\n%s", ERROR, RESET);
>>>>>>> Stashed changes
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

<<<<<<< Updated upstream
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed...\n");
        perror("connect"); 
        exit(0);
    }
    else {
        printf("connected to the server..\n");
        printf("Choose an option:\n1. Register\n2. Login\n");
    } 

    
    func(sockfd);

    
=======
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

>>>>>>> Stashed changes
    close(sockfd);
}