#include <stdio.h>
#include <time.h>
#include <stdlib.h>

<<<<<<< Updated upstream
=======

>>>>>>> Stashed changes
void get_time(char *buffer, size_t buffer_size) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    snprintf(buffer, buffer_size, "| %d-%02d-%02d | %02d:%02d:%02d |\n",
             tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,
             tm.tm_hour, tm.tm_min, tm.tm_sec);
}

int random_range(int min_n, int max_n)
{
    return rand() % (max_n - min_n + 1) + min_n;
}

long int generate_token() {
    long int token = 0;
    int i;
    token = (rand() % 9 + 1);
    for (i = 1; i < 16; i++) {
        token = token * 10 + (rand() % 10);
    }
    return token;
}
void clear_stdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF); // Consumăm caracterele rămase
}
