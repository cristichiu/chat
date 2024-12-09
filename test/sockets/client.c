#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Eroare la creare socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("5.56.109.60");

    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Eroare la conectare");
        exit(EXIT_FAILURE);
    }
    printf("Conectat la server!\n");

    char buffer[1024];
    while (1) {
        printf("Tu: ");
        fgets(buffer, 1024, stdin);
        write(client_socket, buffer, strlen(buffer));
        bzero(buffer, 1024);
        read(client_socket, buffer, 1024);
        printf("Server: %s\n", buffer);
    }

    close(client_socket);
    return 0;
}
