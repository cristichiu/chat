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
    //verification/init
    if(verify(c_users)) return 1063;

    //find last id
    Users buffer;
    FILE *forID = fopen(c_users, "rb");
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
    FILE *file = fopen(c_users, "ab");
    if(file == NULL) return 1062;
    user.deleted = 0;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    user.time_created = tm;
    user.public_id = generate_token();
    fwrite(&user, sizeof(Users), 1, file);
    fclose(file);

    return 0;
}

UserSessions loginUser(char *private_username, char *password, char *IP) {
    //verifications
    UserSessions session;
    if(verify(c_sessions)) return session;

    //find last id
    UserSessions buffer;
    FILE *forID = fopen(c_sessions, "rb");
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

    //find user and identify the user
    FILE *userF = fopen(c_users, "rb");
    if(userF == NULL) return session;
    Users user;
    while(fread(&user, sizeof(Users), 1, userF)) {
        if(!strcmp(user.private_username, private_username) && !strcmp(user.password, password)) {
            session.user_id = user.id;
            break;
        }
    }
    fclose(userF);
    if(session.user_id == NULL) return session;

    //defaults, create
    FILE *file = fopen(c_sessions, "ab");
    if(file == NULL) return session;
    session.token = generate_token();
    strcpy(session.IP, IP);
    session.deleted = 0;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    session.time_created = tm;
    fwrite(&session, sizeof(UserSessions), 1, file);
    fclose(file);

    return session;
}

int createGrup(char *name, int user_id) {
     //verifications
    Grups grup;
    if(verify(c_grups)) return 1064;

    //find last id
    Grups buffer;
    FILE *forID = fopen(c_grups, "rb");
    if(forID == NULL) {
        grup.id = 1;
    } else {
        fseek(forID, 0, SEEK_END);
        long dim = ftell(forID);
        if (dim == 0) {
            grup.id = 1;
        } else {
            fseek(forID, -sizeof(Grups), SEEK_CUR);
            fread(&buffer, sizeof(Grups), 1, forID);
            grup.id = buffer.id + 1;
        }
    }
    fclose(forID);

    //add the owner on GrupMembers file
    GrupMembers owner;
    if(verify(c_grup_members)) return 1064;
    FILE *addOwner = fopen(c_grup_members, "ab");
    if(addOwner == NULL) return 1064;
    owner.user_id = user_id;
    owner.grup_id = grup.id;
    owner.permissions = init+write+read+invite+kick+gPerm;
    owner.accept_by_user = 1;
    fwrite(&owner, sizeof(GrupMembers), 1, addOwner);
    fclose(addOwner);

    //defaults, create
    FILE *file = fopen(c_grups, "ab");
    if(file == NULL) return 1064;
    grup.public_id = generate_token();
    strcpy(grup.name, name);
    grup.deleted = 0;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    grup.time_created = tm;
    grup.owner = user_id;
    fwrite(&grup, sizeof(Grups), 1, file);
    fclose(file);
    return 0;
}

int addUserInGrup(long int grup_public_id, int user_id, long int target_public_id, int permissions) {
    //find grup
    FILE *findGrup = fopen(c_grups, "rb");
    if(findGrup == NULL) return 1055;
    Grups grup;
    while(fread(&grup, sizeof(Grups), 1, findGrup)) if(grup.public_id == grup_public_id) break;
    fclose(findGrup);
    if(grup.public_id != grup_public_id) return 404;
    
    // see user permissions
    int givePermissions = 0;
    int inv = 0;

    if(grup.owner == user_id) { givePermissions = 1; inv = 1; } else {
        FILE *fUsInGrupMem = fopen(c_grup_members, "rb");
        if(fUsInGrupMem == NULL) return 1055;
        GrupMembers grupMember;
        while(fread(&grupMember, sizeof(GrupMembers), 1, fUsInGrupMem)) if(grupMember.user_id == user_id) break;
        fclose(fUsInGrupMem);
        if(grupMember.user_id != user_id) return 404;
        givePermissions = grupMember.permissions%(gPerm*10)/gPerm == 2 ? 1:0;
        inv = grupMember.permissions%(invite*10)/invite == 2 ? 1:0;
    }
    if(!inv) return 403;
    if(!givePermissions) permissions = init;

    // target
    FILE *findTarget = fopen(c_users, "rb");
    if(findTarget == NULL) return 1055;
    Users target;
    while (fread(&target, sizeof(Users), 1, findTarget)) if(target.public_id == target_public_id) break;
    fclose(findTarget);
    if(target.public_id != target_public_id) return 404;

    FILE *findTargetInGrupMembers = fopen(c_grup_members, "rb");
    if(findTargetInGrupMembers == NULL) return 1055;
    GrupMembers grupMember;
    while(fread(&grupMember, sizeof(GrupMembers), 1, findTargetInGrupMembers)) if(grupMember.grup_id == grup.id && grupMember.user_id == target.id) break;
    fclose(findTargetInGrupMembers);
    if(grupMember.grup_id == grup.id && grupMember.user_id == target.id) return 204;

    GrupMembers addTarget;
    if(verify(c_grup_members)) return 1064;
    FILE *addTargetF = fopen(c_grup_members, "ab");
    if(addTargetF == NULL) return 1064;
    addTarget.user_id = target.id;
    addTarget.grup_id = grup.id;
    addTarget.permissions = permissions;
    addTarget.accept_by_user = 0;
    fwrite(&addTarget, sizeof(GrupMembers), 1, addTargetF);
    fclose(addTargetF);
    return 0;
}

Users publicUser(Users user) {
    // protected id, private_username, password
    user.id = NULL;
    strcpy(user.private_username, "");
    strcpy(user.password, "");
    return user;
}

UserSessions publicUserSession(UserSessions userS) {
    // protected id, IP, user_id
    userS.id = NULL;
    strcpy(userS.IP, "");
    userS.user_id = NULL;
    return userS;
}

Grups publicGrup(Grups grup) {
    //protected ID
    grup.id = NULL;
    return grup;
}