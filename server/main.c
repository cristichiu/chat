#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include "./headers/lib.h"
#include "./headers/db/index.h"
#include "./headers/server_utils.h"
#include <arpa/inet.h> 

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

#define MAX_CLIENTS 10
int client_sockets[MAX_CLIENTS] = {0}; 
struct sockaddr_in client_addresses[MAX_CLIENTS]; 

void list_connections() {
    printf("%sActive Connections:%s\n", CYAN, RESET);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_sockets[i] != 0) {
            char *client_ip = inet_ntoa(client_addresses[i].sin_addr);
            int client_port = ntohs(client_addresses[i].sin_port);
            printf("%sClient %d: %s:%d%s\n", WHITE, i + 1, client_ip, client_port, RESET);
        }
    }
}

typedef struct {
    int connfd;
    struct sockaddr_in client_addr;
} client_data;

void handle_message(int connfd, char *buffer);
void *client_thread(void *arg);

typedef struct {
    const char *command;
    void (*handler)(int, char *);
} Command;

Command command_table[] = {
    {"REG:", handle_registration},
    {"LGN:", handle_login},
    {"MSG:", handle_message},
    {"TTT:", list_connections},
    {NULL, NULL}
};

void handle_message(int connfd, char *buffer) {
    list_connections();
    char *message = buffer + strlen("MSG:");
    write(connfd, message, strlen(message));
    printf("%sMessage relayed to client: %s%s\n", CYAN, message, RESET);
}

void handle_command(int connfd, char *buffer) {
    if (buffer == NULL || *buffer == '\0') {
        return;
    }
    for (int i = 0; command_table[i].command != NULL; i++) {
        if (strncmp(buffer, command_table[i].command, strlen(command_table[i].command)) == 0) {
            command_table[i].handler(connfd, buffer);
            return;
        }
    }
}

void *client_thread(void *arg) {
    client_data *data = (client_data *)arg;
    int connfd = data->connfd;
    struct sockaddr_in client_addr = data->client_addr;
    free(data);

    char buff[MAX];
    for (;;) {
        bzero(buff, MAX);
        int n = read(connfd, buff, sizeof(buff));
        if (n <= 0) {
            printf("%sClient disconnected: %s:%d%s\n", ERROR, 
                   inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), RESET);

            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (client_sockets[i] == connfd) {
                    client_sockets[i] = 0;
                    break;
                }
            }

            break;
        }
        handle_command(connfd, buff);
    }
    close(connfd);
    pthread_exit(NULL);
}

int main() {
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;
    pthread_t thread_id;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("%sSocket creation failed...%s\n", ERROR, RESET);
        exit(0);
    } else {
        printf("%sSocket successfully created..%s\n", SUCCESS, RESET);
    }

    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        printf("%sSetting SO_REUSEADDR failed%s\n", ERROR, RESET);
        exit(0);
    }

    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    if ((bind(sockfd, (SA *)&servaddr, sizeof(servaddr))) != 0) {
        printf("%sSocket bind failed...%s\n", ERROR, RESET);
        exit(0);
    } else {
        printf("%sSocket successfully binded..%s\n", SUCCESS, RESET);
    }

    if ((listen(sockfd, MAX_CLIENTS)) != 0) {
        printf("%sListen failed...%s\n", ERROR, RESET);
        exit(0);
    } else {
        printf("%sServer listening..%s\n", SUCCESS, RESET);
    }

    while (1) {
        socklen_t len = sizeof(cli);
        connfd = accept(sockfd, (SA *)&cli, &len);
        if (connfd < 0) {
            printf("%sServer accept failed...%s\n", ERROR, RESET);
            continue;
        } else {
            printf("%sServer accepted a client...%s\n", SUCCESS, RESET);
        }

int index = -1;
for (int i = 0; i < MAX_CLIENTS; i++) {
    if (client_sockets[i] == 0) { // Find an empty slot
        client_sockets[i] = connfd;
        client_addresses[i] = cli;
        index = i;
        break;
    }
}

if (index != -1) {
    client_data *data = (client_data *)malloc(sizeof(client_data));
    data->connfd = connfd;
    data->client_addr = cli;

    pthread_create(&thread_id, NULL, client_thread, (void *)data);
    pthread_detach(thread_id);

    printf("%sClient connected: %s:%d%s\n", SUCCESS, 
           inet_ntoa(cli.sin_addr), ntohs(cli.sin_port), RESET);
} else {
    printf("%sMaximum clients reached. Connection refused.%s\n", ERROR, RESET);
    close(connfd);
}
    }

    close(sockfd);
    return 0;
}
