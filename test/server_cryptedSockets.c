#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define PORT 12345
#define MAX_USERS 5
#define BUFFER_SIZE 256

SSL_CTX *ctx;
SSL *ssl;

// Structura unui utilizator
typedef struct {
    int id;
    char name[50];
} User;

// Baza de date simplificată
User database[MAX_USERS] = {
    {1, "Ion"},
    {2, "Maria"},
    {3, "Ion"},
    {4, "Ana"},
    {5, "Ion"}
};

// Inițializarea OpenSSL
void initialize_openssl() {
    SSL_library_init();
    SSL_load_error_strings();
    OpenSSL_add_all_algorithms();
}

// Crearea contextului SSL
SSL_CTX *create_context() {
    SSL_CTX *ctx = SSL_CTX_new(TLS_server_method());
    if (!ctx) {
        perror("Nu s-a putut crea SSL_CTX");
        exit(EXIT_FAILURE);
    }
    return ctx;
}

// Configurarea certificatului și cheii
void configure_context(SSL_CTX *ctx) {
    if (SSL_CTX_use_certificate_file(ctx, "cert.pem", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    if (SSL_CTX_use_PrivateKey_file(ctx, "key.pem", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
}

// Funcția care răspunde clientului
void *handle_client(void *arg) {
    SSL *ssl = (SSL *)arg;
    char buffer[BUFFER_SIZE];
    int bytes;

    // Continuăm să primim mesaje de la client
    while ((bytes = SSL_read(ssl, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes] = '\0'; // Asigurăm terminarea corectă a stringului

        printf("Mesaj primit de la client: %s\n", buffer);

        // Căutăm utilizatori după nume
        User found_users[MAX_USERS];
        int found_count = 0;

        for (int i = 0; i < MAX_USERS; i++) {
            if (strcmp(database[i].name, buffer) == 0) {
                found_users[found_count++] = database[i];  // Adăugăm utilizatorul în lista de rezultate
            }
        }

        // Dacă am găsit utilizatori, trimitem lista
        if (found_count > 0) {
            SSL_write(ssl, &found_count, sizeof(int));  // Trimitem numărul de utilizatori găsiți
            SSL_write(ssl, found_users, found_count * sizeof(User));  // Trimitem utilizatorii găsiți
        } else {
            // Dacă nu am găsit utilizatori, trimitem un mesaj de eroare
            SSL_write(ssl, &found_count, sizeof(int));  // Trimitem 0, semn că nu s-au găsit utilizatori
        }
    }

    return NULL;
}

int main() {
    initialize_openssl();
    ctx = create_context();
    configure_context(ctx);

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Eroare la crearea socketului server");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Eroare la bind");
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 10) < 0) {
        perror("Eroare la listen");
        exit(EXIT_FAILURE);
    }

    printf("Aștept clienți...\n");

    while (1) {
        int client_socket = accept(server_socket, NULL, NULL);
        if (client_socket < 0) {
            perror("Eroare la acceptare");
            continue;
        }

        // Inițializare SSL pentru client
        ssl = SSL_new(ctx);
        SSL_set_fd(ssl, client_socket);

        if (SSL_accept(ssl) <= 0) {
            ERR_print_errors_fp(stderr);
            close(client_socket);
            continue;
        }

        printf("Conexiune acceptată\n");

        // Creăm un thread pentru a gestiona clientul
        pthread_t client_thread;
        if (pthread_create(&client_thread, NULL, handle_client, ssl) != 0) {
            perror("Eroare la crearea thread-ului client");
            SSL_shutdown(ssl);
            SSL_free(ssl);
            close(client_socket);
            continue;
        }

        // Îl separăm de thread-ul principal
        pthread_detach(client_thread);
    }

    close(server_socket);
    SSL_CTX_free(ctx);
    EVP_cleanup();

    return 0;
}

