#ifndef SERVER_UTILS_H
#define SERVER_UTILS_H

#include <stdio.h>
#include <string.h>


void register_split_string(const char *input, char *username, char *private, char *password);
void login_split_string(const char *input, char *private_username, char *password);
void terminate_connection(int connfd);
void handle_registration(int connfd, char *buffer);
void handle_login(int connfd, char *buffer);
#endif