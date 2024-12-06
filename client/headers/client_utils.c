#include "client_utils.h"


int get_response(int sockfd, char *buffer, int buffer_size) {
  int bytes_read = read(sockfd, buffer, buffer_size);
    if (bytes_read <= 0) {
        return 0;
    }
   buffer[bytes_read] = '\0';
    return 1;
}

bool get_text(const char *expected, int sockfd, char *buffer) {
    get_response(sockfd, buffer, 128); 
    return (strncmp(buffer, expected, strlen(expected)) == 0);
}

void send_command(int sockfd, const char* command, long long token) {
   char message[256]; 
   snprintf(message, sizeof(message), "%s:%lld", command, token);
   write(sockfd, message, strlen(message)); 
}