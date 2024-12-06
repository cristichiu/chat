#include "../index.h"

int verifyDB() {
    struct stat st = {0};
    if (stat(c_DB, &st) == -1) {
        if (mkdir(c_DB, 0777) == 0) printf("The directory %s has been created with success\n", c_DB);
            else { perror("Error at creating the database folder\n"); return 1064; }
    }
    return 0;
}

int verify(char *filePath) {
    //verify for db folder
    if(verifyDB()) return 1064;
    //verify if path exist, if not create
    FILE *verifyForUserFoder = fopen(filePath, "rb");
    if(verifyForUserFoder == NULL) {
        FILE *createUserFile = fopen(filePath, "wb");
        fclose(createUserFile);
    }
    return 0;
}