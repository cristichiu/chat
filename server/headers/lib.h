#ifndef LIB_H_  
#define LIB_H_
#include <stdio.h>

void get_time(char *buffer, size_t buffer_size);  
int random_range(int min_n, int max_n);
long int generate_token();
void clear_stdin();

#endif 