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

#define MAX 80
#define PORT 8080
#define SA struct sockaddr

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
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    char ip_address[16]; 
    snprintf(ip_address, sizeof(ip_address), "127.0.0.%d", 128);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

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

    
    close(sockfd);
}