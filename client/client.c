#include "./headers/action/index.h"
#include "../actions.h"

#define PORT 8080

// ========= MENU ACTIONS =========
Menu a_menu[2][10] = {
    {
        {a_login, "Login", ma_login},
        {a_register, "Register", ma_register},
        {NULL, NULL, NULL},
    },
    {
        {a_whoami, "who am i", ma_whoami},
        {NULL, "log off", ma_logoff},
        {a_create_grup, "creaza un grup", ma_create_grup},
        {NULL, NULL, NULL},
    }
};
// ================================

#define ERROR "\033[0;31m"
#define SUCCESS "\033[0;32m"
#define USER "\033[0;33m"
#define BLUE "\033[0;34m"
#define AUTH "\033[0;35m"
#define CYAN "\033[0;36m"
#define WHITE "\033[0;37m"
#define RESET "\033[0m"

int main() {
    int client_socket;
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

    // pthread_create(&recv_thread, NULL, receive_messages, &client_socket);

    // Trimitem mesaje către server
    char buffer[1024];
    while (1) {
        // ========= HANDLER =========
        int menuIndex = 0;
        FILE *sess = fopen("../db/session.chat", "rb");
        if(sess != NULL) {
            long int token;
            fread(&token, sizeof(long int), 1, sess);
            if(token) menuIndex = 1;
            fclose(sess);
        }
        printf("%sMeniu:\n", CYAN);
        int limit = 0;
        for(int i=0; a_menu[menuIndex][i].instruction != NULL; i++) {
            printf("%d: %s\n", i+1, a_menu[menuIndex][i].instruction);
            limit++;
        }
        printf("%s", RESET);
        int option; scanf("%d", &option);
        if(a_menu[menuIndex][option-1].action != NULL) send(client_socket, a_menu[menuIndex][option-1].action, sizeof(a_menu[menuIndex][option-1].action), 0);
        if(option >= 1 && option <= limit) a_menu[menuIndex][option-1].handler(client_socket);
    }

    close(client_socket);
    return 0;
}
