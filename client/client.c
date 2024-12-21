#include "./headers/action/index.h"
#include "../actions.h"
#include <signal.h>

#define PORT 8080

// ========= MENU ACTIONS =========
Menu a_menu[3][10] = {
    {
        {a_login, "Login", ma_login},
        {a_register, "Register", ma_register},
        {NULL, NULL, NULL},
    },
    {
        {a_whoami, "Who am i", ma_whoami},
        {a_create_grup, "Creaza un grup", ma_create_grup},
        {a_see_my_grups, "Vezi toate grupurile tale", ma_see_my_grups},
        {NULL, "Selecteaza un grup", ma_focus_grup},
        {a_logoff, "Log off", ma_logoff},
        {NULL, NULL, NULL},
    },
    {
        {a_see_focus_grup, "Vezi grupul selectat", ma_see_focus_grup},
        {a_add_new_member, "Adauga un nou membru", ma_add_new_member},
        {a_see_grup_members, "Vezi toti utilizatorii din grup", ma_see_all_grup_members},
        {a_accept_grup_inv, "Acepta invitatia intr-un grup", ma_accept_grup_inv},
        {a_write_message, "Scrie un mesaj in acest grup", ma_write_message},
        {a_see_grup_messages, "Vezi toate mesajele din acest grup", ma_see_all_grup_messages},
        {NULL, "Deselecteaza acest grup", ma_grup_deselect},
        {NULL, NULL, NULL},
    }
};
// ================================

int client_socket;

#define ERROR "\033[0;31m"
#define SUCCESS "\033[0;32m"
#define USER "\033[0;33m"
#define BLUE "\033[0;34m"
#define AUTH "\033[0;35m"
#define CYAN "\033[0;36m"
#define WHITE "\033[0;37m"
#define RESET "\033[0m"

SSL_CTX *ctx;
SSL *ssl;

void init_openssl() {
    SSL_library_init();
    SSL_load_error_strings();
    OpenSSL_add_all_algorithms();
}

SSL_CTX *create_context() {
    SSL_CTX *ctx = SSL_CTX_new(TLS_client_method());
    if(!ctx) {
        perror("SSL_CTX error");
        exit(EXIT_FAILURE);
    }
    return ctx;
}

void handle_sigint(int sig) {
    printf("\nClose...\n");
    close(client_socket);
    SSL_shutdown(ssl);
    SSL_free(ssl);
    SSL_CTX_free(ctx);
    exit(0);
}

int processIn = 0;

void r_handle_save_token(StringRes res) {
    FILE *newSess = fopen(DB_sessions, "wb");
    if(newSess != NULL) {
        long int iToken = ldtoa(res.res);
        fwrite(&iToken, sizeof(long int), 1, newSess);
        fclose(newSess);
    }
}
void r_handle_print(StringRes res) {
    printf("%s\n", res.res);
}
// =========================== TODO RESPONSE MESSAGE HANDLER ===============================
Res res_handler[10] = {
    {r_save_token, r_handle_save_token},
    {r_print, r_handle_print},
    {r_end_wait, NULL},
    {NULL, NULL},
};

void *receive_messages(void *socket) {
    int cl = *(int *)socket;
    char buffer[1024];

    while(1) {
        StringRes res;
        if(verifyConnection(SSL_read(ssl, &res, sizeof(StringRes)), cl, ssl)) break;
        char *arg = strtok(res.args, " ");
        while(arg) {
            if(!strcmp(arg, r_end_wait)) {
                processIn = 0; break;
            }
            for(int i=0; res_handler[i].response != NULL; i++) {
                if(!strcmp(arg, res_handler[i].response)) {
                    res_handler[i].handler(res);
                    break;
                }
            }
            arg = strtok(NULL, " ");
        }
    }
}

int main() {
    init_openssl();
    ctx = create_context();

    signal(SIGINT, handle_sigint);
    struct sockaddr_in server_addr;
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

    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, client_socket);
    if(SSL_connect(ssl) <= 0) {
        ERR_print_errors_fp(stderr);
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    pthread_create(&recv_thread, NULL, receive_messages, &client_socket);

    // Trimitem mesaje către server
    while (1) {
        if(!processIn) {
            char buffer[1024];
            // ========= HANDLER =========
            int menuIndex = 0;
            FILE *sess = fopen(DB_sessions, "rb");
            if(sess != NULL) {
                long int token;
                fread(&token, sizeof(long int), 1, sess);
                if(token) menuIndex = 1;
                fclose(sess);
            }
            FILE *chatSess = fopen(DB_chatSession, "rb");
            if(sess != NULL && menuIndex == 1) {
                long int token;
                fread(&token, sizeof(long int), 1, chatSess);
                if(token) menuIndex = 2;
                fclose(chatSess);
            }
            printf("%sMeniu:\n", CYAN);
            int limit = 0;
            for(int i=0; a_menu[menuIndex][i].instruction != NULL; i++) {
                printf("%d: %s\n", i+1, a_menu[menuIndex][i].instruction);
                limit++;
            }
            printf("%s", RESET);
            int option; scanf("%d", &option);
            if(a_menu[menuIndex][option-1].action != NULL) SSL_write(ssl, a_menu[menuIndex][option-1].action, sizeof(a_menu[menuIndex][option-1].action));
            if(option >= 1 && option <= limit) {
                processIn = 1;
                a_menu[menuIndex][option-1].handler(client_socket, ssl, &processIn);
            }
        }
    }

    printf("Server DOWN");
    SSL_shutdown(ssl);
    SSL_free(ssl);
    SSL_CTX_free(ctx);

    close(client_socket);
    return 0;
}
