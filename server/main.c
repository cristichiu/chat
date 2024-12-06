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
// #include "./headers/db.h"
#define MAX 80
#define PORT 8080
#define SA struct sockaddr

void func(int connfd) 
{ 
    char buff[MAX]; 
    char success[] = "success\n"; 
    char error[] = "error\n"; 

    char ID[17];
    char IP[16];
    char message_content[MAX];
    int n; 
    
    for (;;) { 
        bzero(buff, MAX); 
        read(connfd, buff, sizeof(buff)); 

        if (read <= 0) { 
            printf("Client disconnected or error reading.\n");
            break; 
        }
        // Users test_user;
        // strcpy(test_user.private_username, "admin");
        // strcpy(test_user.username, "admin");
        // strcpy(test_user.password, "admin");
        
        // createUser(test_user);



        printf("From client: %s", buff); 
        write(connfd, success, strlen(success)); 

        if (strncmp("exit", buff, 4) == 0) { 
            printf("Client initiated exit.\n"); 
            break; 
        } 
    } 
}
  

int main() 
{ 
    int sockfd, connfd, len; 
    struct sockaddr_in servaddr, cli; 
  
        // Users test_user;
        // strcpy(test_user.private_username, "admin");
        // strcpy(test_user.username, "admin");
        // strcpy(test_user.password, "admin");
        
        //printf("%i", createUser(test_user));
        // loginUser(1, "100.100.100.1");


    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully created..\n"); 
    bzero(&servaddr, sizeof(servaddr)); 
  
    
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(PORT); 
  
    
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
        printf("socket bind failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully binded..\n"); 
  
    
    if ((listen(sockfd, 5)) != 0) { 
        printf("Listen failed...\n"); 
        exit(0); 
    } 
    else
        printf("Server listening..\n"); 
    len = sizeof(cli); 
  


       while(1) { 
        len = sizeof(cli);
        connfd = accept(sockfd, (SA*)&cli, &len);
        if (connfd < 0) {
            
        } else {
            printf("server accept the client...\n");

            pid_t pid = fork(); 

            if (pid < 0) {
                perror("fork");
                exit(1);
            } else if (pid == 0) { 
                close(sockfd); 
                func(connfd);
                close(connfd);
                exit(0); 
            } else { 
                close(connfd); 
                
                int status;
                waitpid(-1, &status, WNOHANG); 
            }
        }
    }



    connfd = accept(sockfd, (SA*)&cli, &len); 
    if (connfd < 0) { 
        printf("server accept failed...\n"); 
        exit(0); 
    } 
    else
        printf("server accept the client...\n"); 
  
    
    func(connfd); 
  
    
    close(sockfd); 
}