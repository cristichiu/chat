#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Eroare la creare socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Eroare la bind");
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 1) < 0) {
        perror("Eroare la listen");
        exit(EXIT_FAILURE);
    }
    printf("Serverul ascultă pe portul 8080...\n");

    int client_socket = accept(server_socket, NULL, NULL);
    if (client_socket < 0) {
        perror("Eroare la accept");
        exit(EXIT_FAILURE);
    }
    printf("Client conectat!\n");

    char buffer[1024];
    while (1) {
        bzero(buffer, 1024);
        read(client_socket, buffer, 1024);
        printf("Client: %s\n", buffer);
        printf("Tu: ");
        fgets(buffer, 1024, stdin);
        write(client_socket, buffer, strlen(buffer));
    }

    close(client_socket);
    close(server_socket);
    return 0;
}
