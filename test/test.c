#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h> 
#define MAX 80 
#define PORT 8080 
#define SA struct sockaddr 

void chat(int accept_socket) 
{ 
    char buffer[MAX]; 
    int n; 
    for (;;) { 
        bzero(buffer, MAX); 
        read(accept_socket, buffer, sizeof(buffer)); 
        printf("From client: %s\t To client : ", buffer); 
        bzero(buffer, MAX); 
        n = 0; 
        while ((buffer[n++] = getchar()) != '\n'); 
  
        write(accept_socket, buffer, sizeof(buffer)); 
  
        if (strncmp("exit", buffer, 4) == 0) { 
            printf("Server Exit...\n"); 
            break; 
        } 
    } 
} 
  

int main() 
{ 
    int sockfd, accept_socket, len; 
    struct sockaddr_in serveraddress, cli; 
  
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("socket failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket created..\n"); 
    bzero(&serveraddress, sizeof(serveraddress)); 
  
    serveraddress.sin_family = AF_INET; 
    serveraddress.sin_addr.s_addr = htonl(INADDR_ANY); 
    serveraddress.sin_port = htons(PORT); 
  

  
    if ((bind(sockfd, (SA*)&serveraddress, sizeof(serveraddress))) != 0) { 
        printf("bind failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket inded..\n"); 
  
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    if (getsockname(sockfd, (struct sockaddr *)&addr, &addr_len) == -1) {
        perror("getsockname failed");
        exit(0);
    }
    
    char *ip_address = inet_ntoa(addr.sin_addr);
    printf("Server IP Address: %s\n", ip_address);
  
  
    if ((listen(sockfd, 5)) != 0) { 
        printf("Listen failed...\n"); 
        exit(0); 
    } 
    else
        printf("Server listening..\n"); 
    len = sizeof(cli); 
  
    accept_socket = accept(sockfd, (SA*)&cli, &len); 
    if (accept_socket < 0) { 
        printf("accept failed...\n"); 
        exit(0); 
    } 
    else
        printf("accept the client...\n"); 
    
    chat(accept_socket); 
    close(sockfd); 
}