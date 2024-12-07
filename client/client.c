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

#define ERROR "\033[0;31m"
#define SUCCESS "\033[0;32m"
#define USER "\033[0;33m"
#define BLUE "\033[0;34m"
#define AUTH "\033[0;35m"
#define CYAN "\033[0;36m"
#define WHITE "\033[0;37m"
#define RESET "\033[0m"

long long token = 0;

void handle_registration_request(int sockfd) {
    printf("\n%s######  Registration requested by server  ######%s\n", CYAN, RESET);

    char username[MAX], private_username[MAX], password[MAX];

    printf("%sEnter Username: %s", AUTH, RESET);
    bzero(username, sizeof(username));
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0'; 

    printf("%sEnter Private Username: %s", AUTH, RESET);
    bzero(private_username, sizeof(private_username));
    fgets(private_username, sizeof(private_username), stdin);
    private_username[strcspn(private_username, "\n")] = '\0'; 

    printf("%sEnter Password: %s", AUTH, RESET);
    bzero(password, sizeof(password));
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0'; 

    char reg_data[MAX];
    // >!# used for delimiting the string 
    snprintf(reg_data, sizeof(reg_data), "REG:%s>!#%s>!#%s", username, private_username, password);
    write(sockfd, reg_data, sizeof(reg_data));

    printf("%s######  Registration data sent to server  ######%s\n", CYAN, RESET);
}

void handle_login_request(int sockfd) {
    printf("\n%s######  Login requested by server  ######%s\n", CYAN, RESET);

    char private_username[MAX], password[MAX];

    printf("%sEnter Private Username: %s", AUTH, RESET);
    bzero(private_username, sizeof(private_username));
    fgets(private_username, sizeof(private_username), stdin);
    private_username[strcspn(private_username, "\n")] = '\0'; 

    printf("%sEnter Password: %s", AUTH, RESET);
    bzero(password, sizeof(password));
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0'; 

    char reg_data[MAX];
    // >!# used for delimiting the string 
    snprintf(reg_data, sizeof(reg_data), "LGN:%s>!#%s", private_username, password);
    write(sockfd, reg_data, sizeof(reg_data));

    printf("%s######  Login data sent to server  ######%s\n", CYAN, RESET);
}

void handle_client(int sockfd, int connfd)
{
    char buff[MAX];
    int n;
    int wait_response = 0;
    for (;;) {
        bzero(buff, sizeof(buff));

        if (wait_response == 0) {
        printf("%sEnter  : %s", USER, RESET);
        n = 0;
        while ((buff[n++] = getchar()) != '\n');

        write(sockfd, buff, sizeof(buff));
        bzero(buff, sizeof(buff));
        }

        read(sockfd, buff, sizeof(buff));
        printf("%sServer : %s%s%s\n", CYAN, WHITE, buff, RESET);


        if ((strncmp(buff, "reg_request", strlen("reg_request"))) == 0) {
               handle_registration_request(sockfd);
               //wait_response = 1;
        } else if ((strncmp(buff, "lgn_request", strlen("lgn_request"))) == 0) {
               handle_login_request(sockfd);
               wait_response = 1;
        } else if ((strncmp(buff, "TKN", 3)) == 0) {
            token = atoll(buff + 4);
            wait_response = 0;
        } else if ((strncmp(buff, "ex", 2)) == 0) {
            printf("%sClient Exit...\n%s", ERROR, RESET);
            close(sockfd);
            close(connfd);
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

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("%ssocket creation failed...\n%s", ERROR, RESET);
        exit(0);
    }
    else
        printf("%sSocket successfully created..\n%s", SUCCESS, RESET);
    bzero(&servaddr, sizeof(servaddr));

    char ip_address[16]; 
    snprintf(ip_address, sizeof(ip_address), "127.0.0.%d", 128);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("%sconnection with the server failed...\n%s", ERROR, RESET);
        perror("connect"); 
        exit(0);
    }
    else {
        printf("%sconnected to the server..\n%s", SUCCESS, RESET);
        printf("%sChoose an option:\n1. Register\n2. Login\n%s", USER, RESET);
    } 

    // function for chat
    handle_client(sockfd, connfd);

    // close the socket
    close(sockfd);
}
