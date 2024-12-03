#include "db.h"

int verifyDB() {
    struct stat st = {0};
    if (stat(c_DB, &st) == -1) {
        if (mkdir(c_DB, 0777) == 0) printf("The directory %s has been created with success\n", c_DB);
            else { perror("Error at creating the database folder\n"); return 1064; }
    }
    return 0;
}

char *createUser(Users user) {
    //verify for db folder
    if(verifyDB()) return "Ceva nu a mers bine";
    //path for user file
    char *userFile = malloc(32);
    sprintf(userFile, "%s/%s", c_DB, "user.chat");
    //verify if path exist, if not create
    FILE *verifyForUserFoder = fopen(userFile, "rb");
    if(verifyForUserFoder == NULL) {
        FILE *createUserFile = fopen(userFile, "wb");
        fclose(createUserFile);
    }
    //find the ID
    Users buffer;
    FILE *forID = fopen(userFile, "rb");
    if(forID == NULL) {
        user.id = 1;
    } else {
        fseek(forID, 0, SEEK_END);
        long dim = ftell(forID);
        if (dim == 0) {
            user.id = 1;
        } else {
            fseek(forID, -sizeof(Users), SEEK_CUR);
            fread(&buffer, sizeof(Users), 1, forID);
            user.id = buffer.id + 1;
        }
    }
    fclose(forID);
    //create user
    FILE *file = fopen(userFile, "ab");
    if(file == NULL) return "Something went wrong, this is a bug, report this.";
    user.deleted = 0;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    user.time_created = tm;
    fwrite(&user, sizeof(Users), 1, file);
    fclose(file);

    free(userFile);
    return "SUCCESS";
}

UserSessions loginUser(Users user) {
    
}