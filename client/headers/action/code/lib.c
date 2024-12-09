#include "../lib.h"

long int ldtoa(char *str) {
    long int res = 0;
    for(int i=0; i<strlen(str); i++) {
        res = res*10+(str[i]-'0');
    }
    return res;
}

void verifyConnection(int rs, int cl) {
    if(rs <= 0) {
        printf("Client deconectat, socket: %d\n", cl);
        close(cl);
    }
}