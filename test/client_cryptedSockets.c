#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define PORT 12345
#define BUFFER_SIZE 256

SSL_CTX *ctx;
SSL *ssl;

typedef struct {
    int id;
    char name[50];
} User;
// Inițializarea OpenSSL
void initialize_openssl() {
    SSL_library_init();
    SSL_load_error_strings();
    OpenSSL_add_all_algorithms();
}

// Crearea contextului SSL
SSL_CTX *create_context() {
    SSL_CTX *ctx = SSL_CTX_new(TLS_client_method());
    if (!ctx) {
        perror("Nu s-a putut crea SSL_CTX");
        exit(EXIT_FAILURE);
    }
    return ctx;
}

// Configurarea certificatelor SSL (opțional pentru client, dar necesar pentru a verifica serverul)
void configure_context(SSL_CTX *ctx) {
    // Pentru client, nu este necesar să configurăm certificatele serverului
    // Dacă dorim să validăm serverul, am putea folosi SSL_CTX_load_verify_locations
}

// Funcția pentru a citi și afișa utilizatorii primiți de la server
void handle_server_response() {
    int num_users;
    SSL_read(ssl, &num_users, sizeof(int));  // Citim numărul de utilizatori

    if (num_users > 0) {
        // Dacă au fost găsiți utilizatori, îi citim și afișăm
        User users[num_users];
        SSL_read(ssl, users, num_users * sizeof(User));

        printf("Am găsit %d utilizatori:\n", num_users);
        for (int i = 0; i < num_users; i++) {
            printf("ID: %d, Nume: %s\n", users[i].id, users[i].name);
        }
    } else {
        // Dacă nu au fost găsiți utilizatori, afișăm un mesaj de eroare
        printf("Nu au fost găsiți utilizatori cu acest nume.\n");
    }
}

int main() {
    // Inițializarea OpenSSL și crearea contextului SSL
    initialize_openssl();
    ctx = create_context();
    configure_context(ctx);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Eroare la crearea socketului client");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Eroare la conectarea la server");
        exit(EXIT_FAILURE);
    }

    // Inițializare SSL pentru client
    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, sock);

    if (SSL_connect(ssl) <= 0) {
        ERR_print_errors_fp(stderr);
        close(sock);
        exit(EXIT_FAILURE);
    }

    printf("Conectat la server\n");

    // Trimitem un nume către server
    char name[BUFFER_SIZE];
    printf("Introdu numele pentru care vrei să găsești utilizatori: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';  // Eliminăm caracterul '\n' din input

    // Trimitem numele criptat către server
    SSL_write(ssl, name, strlen(name));

    // Așteptăm răspunsul serverului
    handle_server_response();

    // Închidem conexiunea
    SSL_shutdown(ssl);
    SSL_free(ssl);
    close(sock);
    SSL_CTX_free(ctx);

    return 0;
}

