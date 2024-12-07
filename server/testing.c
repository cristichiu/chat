#include "./headers/db/index.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <signal.h>
#include <errno.h>

#define PORT 8080
#define MAX_CLIENTS 100

int server_socket, client_sockets[MAX_CLIENTS];

// Funcția de oprire sigură a serverului la Ctrl+C
void handle_sigint(int sig) {
    printf("\nServerul se oprește...\n");

    // Închidem toate conexiunile active
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_sockets[i] != 0) {
            close(client_sockets[i]);
        }
    }
    close(server_socket); // Închidem socket-ul serverului
    exit(0);
}

long int ldtoa(char *str) {
    long int res = 0;
    for(int i=0; i<strlen(str); i++) {
        res = res*10+(str[i]-'0');
    }
    return res;
}

int main() {
    int new_socket, max_sd, activity;
    struct sockaddr_in server_addr;
    char buffer[1024];
    fd_set readfds;

    // Setăm handler-ul pentru SIGINT
    signal(SIGINT, handle_sigint);

    // Inițializăm socket-ul serverului
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Permitem reutilizarea portului cu SO_REUSEADDR
    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Asociem socket-ul cu portul
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Pornim serverul în modul de ascultare
    if (listen(server_socket, 10) < 0) {
        perror("Listen failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Serverul rulează pe portul %d...\n", PORT);

    // Inițializăm array-ul pentru clienți
    for (int i = 0; i < MAX_CLIENTS; i++) {
        client_sockets[i] = 0;
    }

    while (1) {
        // Resetează setul de socket-uri
        FD_ZERO(&readfds);
        FD_SET(server_socket, &readfds); // Adăugăm serverul în set
        max_sd = server_socket;

        // Adăugăm socket-urile clienților activi
        for (int i = 0; i < MAX_CLIENTS; i++) {
            int sd = client_sockets[i];
            if (sd > 0) FD_SET(sd, &readfds);
            if (sd > max_sd) max_sd = sd;
        }

        // Așteptăm activitate pe socket-uri
        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
        if (activity < 0 && errno != EINTR) {
            perror("Eroare la select()");
        }

        // Verificăm conexiuni noi
        if (FD_ISSET(server_socket, &readfds)) {
            new_socket = accept(server_socket, NULL, NULL);
            if (new_socket < 0) {
                perror("Accept failed");
                continue;
            }

            printf("Client nou conectat, socket: %d\n", new_socket);

            // Adăugăm clientul în lista de conexiuni
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (client_sockets[i] == 0) {
                    client_sockets[i] = new_socket;
                    break;
                }
            }
        }

        // Verificăm activitatea clienților existenți
        for (int i = 0; i < MAX_CLIENTS; i++) {
            int sd = client_sockets[i];

            if (FD_ISSET(sd, &readfds)) {
                int valread = read(sd, buffer, sizeof(buffer));
                if (valread == 0) {
                    // Clientul s-a deconectat
                    printf("Client deconectat, socket: %d\n", sd);
                    close(sd);
                    client_sockets[i] = 0;
                } else {
                    buffer[valread] = '\0';
                    char *token = strtok(buffer, "<!>");
                    if(!strcmp(token, "register")) {
                        token = strtok(NULL, "<!>");
                        if(token == NULL) {
                            send(sd, "Nu ai scris username-ul\n", 100, 0);
                            break;
                        }
                        char username[64];
                        strcpy(username, token);
                        token = strtok(NULL, "<!>");
                        if(token == NULL) {
                            send(sd, "Nu ai scris private username-ul\n", 100, 0);
                            break;
                        }
                        char private_username[64];
                        strcpy(private_username, token);
                        token = strtok(NULL, "<!>");
                        if(token == NULL) {
                            send(sd, "Nu ai scris password-ul\n", 100, 0);
                            break;
                        }
                        char password[64];
                        strcpy(password, token);

                        if(createUser(private_username, username, password)) {
                            send(sd, "Ceva nu a mers bine, nu am putut crea utilizatorul\n", 100, 0);
                        } else send(sd, "User creat cu succes\n", 100, 0);
                    }
                    if(!strcmp(token, "login")) {
                        token = strtok(NULL, "<!>");
                        if(token == NULL) {
                            send(sd, "Nu ai scris private username-ul\n", 100, 0);
                            break;
                        }
                        char private_username[64];
                        strcpy(private_username, token);
                        token = strtok(NULL, "<!>");
                        if(token == NULL) {
                            send(sd, "Nu ai scris password-ul\n", 100, 0);
                            break;
                        }
                        char password[64];
                        strcpy(password, token);

                        UserSessions login = loginUser(private_username, password, "todo");
                        if(!login.id) {
                            send(sd, "Asa utilizator nu exista\n", 100, 0);
                        } else {
                            char token[100];
                            sprintf(token, "session<!>%ld", login.token);
                            send(sd, token, 100, 0);
                        }
                    }
                    if(!strcmp(token, "auth")) {
                        token = strtok(NULL, "<!>");
                        if(token == NULL) {
                            printf("Nu esti autentificat");
                            break;
                        }
                        UserSessions session = getUserSessionByToken(ldtoa(token));
                        if(!session.id) { send(sd, "Ceva nu a mers bine, nu am putut gasi sesiunea ta!\n", 100, 0); break; }
                        Users user = getUserByLInt(session.user_id, US_FOR_ID);
                        if(!user.id) { send(sd, "Ceva nu a mers bine nu am putut gasi userul dupa sesiune\n", 100, 0); break; }
                        token = strtok(NULL, "<!>");
                        if(!strcmp(token, "whoami")) {
                            char res[100];
                            sprintf(res, "Esti cumva asta tu?: %ld - %ld - %s\n", user.public_id, session.token, user.username);
                            send(sd, res, 100, 0);
                        }
                    }
                }
            }
        }
    }

    return 0;
}
