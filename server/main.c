#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h> 
#define MAX 80
#define PORT 8081
#define SA struct sockaddr

void func(int connfd) 
{ 
    char buff[MAX]; 
    char success[] = "success\n"; 
    char error[] = "error\n"; 
    long int ID[17]; 
    char message_content[4];
    int n; 
    
    for (;;) { 
        bzero(buff, MAX); 
        read(connfd, buff, sizeof(buff)); 

        if (read <= 0) { 
            printf("Client disconnected or error reading.\n");
            break; 
        }

        printf("From client: %s", buff); 

    strncpy(ID, buff, 16);
    ID[16] = '\0'; 


    strncpy(message_content, buff + 16, 4);
    message_content[4] = '\0'; 

    printf("First part: %s\n", ID);
    printf("Second part: %s\n", message_content);


        write(connfd, success, strlen(success)); 

        if (strncmp("exit", message_content, 4) == 0) { 
            printf("Client initiated exit.\n"); 
            break; 
        } 
    } 
}
  

int main() 
{ 
    int sockfd, connfd, len; 
    struct sockaddr_in servaddr, cli; 
  
    
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