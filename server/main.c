#include "../actions.h"
#include "./headers/db/index.h"
#include "./headers/socket/index.h"

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

// ================ COMMANDS ================
Command command_table[] = {
    {a_register, handle_register},
    {a_login, handle_login},
    {a_whoami, handle_whoami},
    {a_create_grup, handle_create_grup},
    {NULL, NULL}
};
// ==========================================

int main() {
    showTable(SHOW_GRUPS);
    int new_socket, max_sd, activity;
    struct sockaddr_in server_addr;
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
                char buffer[1024];
                int valread = read(sd, buffer, sizeof(buffer));
                if (valread == 0) {
                    // Clientul s-a deconectat
                    printf("Client deconectat, socket: %d\n", sd);
                    close(sd);
                    client_sockets[i] = 0;
                } else {
                    buffer[valread] = '\0';
                    // ============= COMMAND HANDLER =============
                    for (int j = 0; command_table[j].command != NULL; j++) {
                        if (!strcmp(buffer, command_table[j].command)) {
                            command_table[j].handler(sd, &client_sockets[i]);
                            break;
                        }
                    }
                    // LOGS
                    FILE *logs = fopen("../logs/activityRead.log", "a");
                    if(logs != NULL) {
                        time_t t = time(NULL);
                        struct tm tm = *localtime(&t);
                        char log[1212];
                        sprintf(log, "D[%d-%02d-%02d] T[%02d:%02d:%02d] M[%s]\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, buffer);
                        fputs(log, logs);
                        fclose(logs);
                    } else {
                        printf("Logs not working properly\n");
                    }
                }
            }
        }
    }

    return 0;
}
