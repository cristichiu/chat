#include "../actions.h"
#include "./headers/db/index.h"
#include "./headers/socket/index.h"

#define PORT 8080
#define MAX_CLIENTS 100

int server_socket;
SSL_CTX *ctx;
Client client_sockets[MAX_CLIENTS];

// Funcția de oprire sigură a serverului la Ctrl+C
void handle_sigint(int sig) {
    printf("\nServerul se oprește...\n");

    // Închidem toate conexiunile active
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_sockets[i].socket != 0) {
            close(client_sockets[i].socket);
            SSL_shutdown(client_sockets[i].ssl);
            SSL_free(client_sockets[i].ssl);
        }
    }
    close(server_socket); // Închidem socket-ul serverului
    SSL_CTX_free(ctx);
    EVP_cleanup();
    exit(0);
}

// ================ COMMANDS ================
Command command_table[] = {
    {a_register, handle_register},
    {a_login, handle_login},
    {a_whoami, handle_whoami},
    {a_create_grup, handle_create_grup},
    {a_logoff, handle_logoff},
    {a_see_my_grups, handle_see_my_grups},
    {a_see_focus_grup, handle_see_focus_grup},
    {a_add_new_member, handle_add_new_meber},
    {a_see_grup_members, handle_see_grup_members},
    {a_write_message, handle_write_message},
    {a_accept_grup_inv, handle_accept_grup_inv},
    {a_see_grup_messages, handle_see_grup_messages},
    {NULL, NULL}
};
// ==========================================

// ======== SSL encryption ========
void init_openssl() {
    SSL_library_init();
    SSL_load_error_strings();
    OpenSSL_add_all_algorithms();
}

SSL_CTX *create_context() {
    SSL_CTX *ctx = SSL_CTX_new(TLS_server_method());
    if(!ctx) {
        perror("SLL_CTX error");
        exit(EXIT_FAILURE);
    }
    return ctx;
}

void context_config(SSL_CTX *ctx) {
    if(SSL_CTX_use_certificate_file(ctx, "../ssl/cert.pem", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
    if(SSL_CTX_use_PrivateKey_file(ctx, "../ssl/key.pem", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
}

int main() {
    srand(time(NULL));
    int new_socket, max_sd, activity;
    struct sockaddr_in server_addr;
    fd_set readfds;

    init_openssl();
    ctx = create_context();
    context_config(ctx);

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
        client_sockets[i].socket = 0;
        client_sockets[i].ssl = NULL;
    }

    while (1) {
        // Resetează setul de socket-uri
        FD_ZERO(&readfds);
        FD_SET(server_socket, &readfds); // Adăugăm serverul în set
        max_sd = server_socket;

        // Adăugăm socket-urile clienților activi
        for (int i = 0; i < MAX_CLIENTS; i++) {
            int sd = client_sockets[i].socket;
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
                if (client_sockets[i].socket == 0) {
                    client_sockets[i].socket = new_socket;
                    client_sockets[i].ssl = SSL_new(ctx);
                    SSL_set_fd(client_sockets[i].ssl, new_socket);
                    if(SSL_accept(client_sockets[i].ssl) <= 0) {
                        ERR_print_errors_fp(stderr);
                        close(new_socket);
                        SSL_shutdown(client_sockets[i].ssl);
                        SSL_free(client_sockets[i].ssl);
                        client_sockets[i].ssl = NULL;
                        client_sockets[i].socket = 0;
                        continue;
                    }
                    break;
                }
            }
        }

        // Verificăm activitatea clienților existenți
        for (int i = 0; i < MAX_CLIENTS; i++) {
            int sd = client_sockets[i].socket;
            SSL *sl = client_sockets[i].ssl;

            if (FD_ISSET(sd, &readfds)) {
                char buffer[1024];
                int valread = SSL_read(sl, buffer, sizeof(buffer));
                if (valread == 0) {
                    // Clientul s-a deconectat
                    printf("Client deconectat, socket: %d\n", sd);
                    close(sd);
                    SSL_shutdown(client_sockets[i].ssl);
                    SSL_free(client_sockets[i].ssl);
                    client_sockets[i].ssl = NULL;
                    client_sockets[i].socket = 0;
                } else {
                    buffer[valread] = '\0';
                    // ============= COMMAND HANDLER =============
                    for (int j = 0; command_table[j].command != NULL; j++) {
                        if (!strcmp(buffer, command_table[j].command)) {
                            command_table[j].handler(&client_sockets[i]);
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
