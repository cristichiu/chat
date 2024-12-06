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

void showTable(ShowTableOption option) {
    switch(option) {
        case SHOW_USERS: {
            FILE *file = fopen(c_users, "rb");
            if(file == NULL) { printf("File not found."); break; }
            Users buffer;
            printf("ID | PUBLIC_ID | PRIVATE_USERNAME | PASSWORD | USERNAME | DELETED\n");
            while(fread(&buffer, sizeof(Users), 1, file)) {
                printf("%ld | %ld | %s | %s | %s | %d\n", buffer.id, buffer.public_id, buffer.private_username, buffer.password, buffer.username, buffer.deleted);
            }
            fclose(file);
            break;
        }
        case SHOW_GRUPS: {
            FILE *file = fopen(c_grups, "rb");
            if(file == NULL) { printf("File not found."); break; }
            Grups buffer;
            printf("ID | PUBLIC_ID | OWNER | NAME | DELETED\n");
            while(fread(&buffer, sizeof(Grups), 1, file)) {
                printf("%ld | %ld | %ld | %s | %d\n", buffer.id, buffer.public_id, buffer.owner, buffer.name, buffer.deleted);
            }
            fclose(file);
            break;
        }
        case SHOW_GRUP_MEMBERS: {
            FILE *file = fopen(c_grup_members, "rb");
            if(file == NULL) { printf("File not found."); break; }
            GrupMembers buffer;
            printf("USER_ID | GRUP_ID | ACCEPTED_BY_USER | PERMISSIONS | DELETED\n");
            while(fread(&buffer, sizeof(GrupMembers), 1, file)) {
                printf("%ld | %ld | %d | %d | %d\n", buffer.user_id, buffer.grup_id, buffer.accept_by_user, buffer.permissions, buffer.deleted);
            }
            fclose(file);
            break;
        }
        case SHOW_MESSAGES: {
            FILE *file = fopen(c_grup_messages, "rb");
            if(file == NULL) { printf("File not found."); break; }
            Messages buffer;
            printf("ID | PUBLIC_ID | GRUP_ID | USER_ID | MESSAGE | DELETED\n");
            while(fread(&buffer, sizeof(Messages), 1, file)) {
                printf("%ld | %ld | %ld | %ld | %s | %d\n", buffer.id, buffer.public_id, buffer.grup_id, buffer.user_id, buffer.message, buffer.deleted);
            }
            fclose(file);
            break;
        }
        case SHOW_SESSIONS: {
            FILE *file = fopen(c_sessions, "rb");
            if(file == NULL) { printf("File not found."); break; }
            UserSessions buffer;
            printf("ID | USER_ID | TOKEN | IP | DELETED\n");
            while(fread(&buffer, sizeof(UserSessions), 1, file)) {
                printf("%ld | %ld | %ld | %s | %d\n", buffer.id, buffer.user_id, buffer.token, buffer.IP, buffer.deleted);
            }
            fclose(file);
            break;
        }
    }
}