#include "db.h"

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

int createUser(Users user) {
    char *userFile = malloc(32);
    sprintf(userFile, "%s/%s", c_DB, "users.chat");
    if(verify(userFile)) return 1063;
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
    FILE *file = fopen(userFile, "ab");
    if(file == NULL) return 1062;
    user.deleted = 0;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    user.time_created = tm;
    fwrite(&user, sizeof(Users), 1, file);
    fclose(file);
    free(userFile);
    return 0;
}

UserSessions loginUser(int user, char *IP) {
    //verifications
    UserSessions session;
    char *sessionFile = malloc(32);
    sprintf(sessionFile, "%s/%s", c_DB, "sessions.chat");
    if(verify(sessionFile)) return session;
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
    session.user_id = user;
    session.deleted = 0;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    session.time_created = tm;
    fwrite(&session, sizeof(UserSessions), 1, file);
    fclose(file);

    free(sessionFile);
    return session;
}