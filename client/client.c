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

int c_random(int min_n, int max_n)
{
    return rand() % (max_n - min_n + 1) + min_n;
}

long int generate_id()
{
    long int min_n = 1000000000000000; 
    long int max_n = 9999999999999999;
    return rand() % (max_n - min_n + 1) + min_n;
}

void func(int sockfd)
{
    char buff[MAX];
    char message_content[MAX] = "";
    char id[17]; 
    long int id_integer = generate_id();
    int n;
    snprintf(id, 17, "%li", id_integer);

    for (;;) {
        bzero(buff, sizeof(buff));
        printf("Enter the string : ");
        n = 0;
        while ((buff[n++] = getchar()) != '\n');


        strncat(message_content, id, MAX - strlen(message_content) - 1);
        strncat(message_content, buff, MAX - strlen(message_content) - 1);
        write(sockfd, message_content, sizeof(buff));
        memset(message_content, 0, sizeof(message_content));

        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
        printf("From Server : %s", buff);
        if ((strncmp(buff, "exit", 4)) == 0) {
            printf("Client Exit...\n");
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
    snprintf(ip_address, sizeof(ip_address), "127.0.0.%d", c_random(1,255));

    printf("127.0.0.%d", c_random(1,255));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed...\n");
        perror("connect"); 
        exit(0);
    }
    else
        printf("connected to the server..\n");

    
    func(sockfd);

    
    close(sockfd);
}