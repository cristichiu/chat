#include <time.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include "./lib.h"

#define c_DB "../DB"

typedef struct Users {
    int id;
    char private_username[64];
    char password[64];
    char username[64];
    short int deleted;
    struct tm time_created;
} Users;

typedef struct UserSessions {
    int id;
    char IP[32];
    long int token;
    short int deleted;
    int user_id;
    struct tm time_created;
} UserSessions;

typedef struct Messages {
    int id;
    char message[1024];
    short int deleted;
    short int dm;
    int user_id;
    struct tm time_created;
} Messages;

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

char *createUser(Users user) {
    char *userFile = malloc(32);
    sprintf(userFile, "%s/%s", c_DB, "users.chat");
    if(!verify(userFile)) return "Ceva nu a mers bine";
    //find last id
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
    printf("test1");
    FILE *file = fopen(userFile, "ab");
    if(file == NULL) return "Something went wrong, this is a bug, report this.";
    user.deleted = 0;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    user.time_created = tm;
    fwrite(&user, sizeof(Users), 1, file);
    fclose(file);
    free(userFile);
    printf("test2");
    return "SUCCESS";
}

UserSessions loginUser(Users user, char *IP) {
    //verifications
    UserSessions session;
    char *sessionFile = malloc(32);
    sprintf(sessionFile, "%s/%s", c_DB, "sessions.chat");
    if(!verify(sessionFile)) return session;
    //last id
    UserSessions buffer;
    FILE *forID = fopen(sessionFile, "rb");
    if(forID == NULL) {
        session.id = 1;
    } else {
        fseek(forID, 0, SEEK_END);
        long dim = ftell(forID);
        if (dim == 0) {
            session.id = 1;
        } else {
            fseek(forID, -sizeof(UserSessions), SEEK_CUR);
            fread(&buffer, sizeof(UserSessions), 1, forID);
            session.id = buffer.id + 1;
        }
    }
    fclose(forID);
    //create session
    FILE *file = fopen(sessionFile, "ab");
    if(file == NULL) return session;
    //defaults
    session.token = generate_token();
    strcpy(session.IP, IP);
    session.deleted = 0;
    session.user_id = user.id;
    session.deleted = 0;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    session.time_created = tm;
    fwrite(&session, sizeof(UserSessions), 1, file);
    fclose(file);

    free(sessionFile);
    return session;
}