#ifndef CLIENT_UTILS_H
#define CLIENT_UTILS_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>  // For bool type


int get_response(int sockfd, char *buffer, int buffer_size);
bool get_text(const char *expected, int sockfd, char *buffer);
void send_command(int sockfd, const char* command, long long token); // Example


#endif