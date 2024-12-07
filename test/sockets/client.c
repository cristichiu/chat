#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080

typedef struct UserSession {
    long int token;
    long int focus_grup_id;
} UserSession;

long int ldtoa(char *str) {
    long int res = 0;
    for(int i=0; i<strlen(str); i++) {
        res = res*10+(str[i]-'0');
    }
    return res;
}

void *receive_messages(void *socket) {
    int client_socket = *(int *)socket;
    char buffer[1024];

    while (1) {
        int valread = read(client_socket, buffer, sizeof(buffer));
        if (valread <= 0) {
            printf("Conexiunea cu serverul a fost pierdută.\n");
            close(client_socket);
            exit(1);
        }

        buffer[valread] = '\0';
        char *token = strtok(buffer, "<!>");
        if(!strcmp(token, "session")) {
            token = strtok(NULL, "<!>");
            if(token != NULL) {
                FILE *update = fopen("db.chat", "r+b");
                if(update == NULL) {
                    FILE *create = fopen("db.chat", "wb");
                    UserSession cSess;
                    cSess.token = ldtoa(token);
                    cSess.focus_grup_id = 0;
                    fwrite(&cSess, sizeof(UserSession), 1, create);
                    fclose(create);
                } else {
                    UserSession uSess;
                    fread(&uSess, sizeof(UserSession), 1, update);
                    fseek(update, -sizeof(UserSession), SEEK_CUR);
                    uSess.token = ldtoa(token);
                    fwrite(&uSess, sizeof(UserSession), 1, update);
                    fclose(update);
                }
                printf("Te-ai logat cu succes\n");
            } else printf("Ceva nu a mers bine la login.\n");
        } else printf("%s\n", buffer);
    }
}

int main() {
    FILE *file = fopen("db.chat", "rb");
    if(file != NULL) {
        UserSession sess;
        fread(&sess, sizeof(UserSession), 1, file);
        printf("%ld\n", sess.token);
        fclose(file);
    }
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[1024], name[50];
    pthread_t recv_thread;

    // Creăm socket-ul
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Conectăm clientul la server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connect failed");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    printf("Conectat la server.\nIntrodu numele tău: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0; // Eliminăm '\n' din input

    // Trimitem numele către server
    send(client_socket, name, strlen(name), 0);

    // Pornim thread-ul pentru a recepționa mesaje
    pthread_create(&recv_thread, NULL, receive_messages, &client_socket);

    // Trimitem mesaje către server
    while (1) {
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0; // Eliminăm '\n'
        send(client_socket, buffer, strlen(buffer), 0);
    }

    close(client_socket);
    return 0;
}
