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

#define MAX 80
#define PORT 8080
#define SA struct sockaddr


#include <stdio.h>
#include <string.h>

void register_split_string(const char *input, char *username, char *private, char *password) {
    char buffer[64];
    strncpy(buffer, input, sizeof(buffer));
    buffer[sizeof(buffer) - 1] = '\0'; 

    
    char *token = strtok(buffer, "#>!");
    
    
    if (token != NULL) {
        strncpy(username, token, 64);
        username[255] = '\0'; 
        token = strtok(NULL, "#>!");
    }

    
    if (token != NULL) {
        strncpy(private, token, 64);
        private[255] = '\0'; 
        token = strtok(NULL, "#>!");
    }

    
    if (token != NULL) {
        strncpy(password, token, 64);
        password[255] = '\0'; 
    }
}


void terminate_connection (int connfd) {
        write(connfd, "ex", strlen("ex"));
        close(connfd);
        exit(0);
}

void handle_client(int connfd, int sockfd) {
    char buff[MAX];
    char success[] = "success\n";
    char register_request[] = "reg_request";
    char error[] = "error\n";
    int n;
    char username[64];
    char private_username[64];
    char password[64];
    for (;;) {
        bzero(buff, MAX);
        read(connfd, buff, sizeof(buff));

        if (read <= 0) {
            printf("Client disconnected or error reading.\n");
            break;
        }

        if (strncmp("1", buff, strlen("1")) == 0) {
            write(connfd, register_request, strlen(register_request));



        } else if (strncmp("2", buff, strlen("2")) == 0) {
            write(connfd, success, strlen(success));
        } else {
            register_split_string(buff, username, private_username, password);
            printf("Username: %s\n", username);
            printf("Private: %s\n", private_username);
            printf("Password: %s\n", password);

            terminate_connection(connfd);
        }

        if (strncmp("ex", buff, 2) == 0) {
            terminate_connection(connfd);
            break;
        }
    }
}

int main() {
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    } else
        printf("Socket successfully created..\n");
    bzero( & servaddr, sizeof(servaddr));


    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);


    if ((bind(sockfd, (SA * ) & servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    } else
        printf("Socket successfully binded..\n");


    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    } else
        printf("Server listening..\n");
    len = sizeof(cli);

    connfd = accept(sockfd, (SA * ) & cli, & len);
    if (connfd < 0) {
        printf("server accept failed...\n");
        exit(0);
    } else {
        printf("server accept the client...\n");
    }

    handle_client(connfd, sockfd);


    close(sockfd);
}