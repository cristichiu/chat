#include "../index.h"

int createUser(char *private_username, char *username, char *password) {
    Users user;

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
        fclose(forID);
    }
    
    //create user
    FILE *file = fopen(c_users, "ab");
    if(file == NULL) return 1062;
    user.deleted = 0;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    user.time_created = tm;
    user.public_id = generate_token();
    strcpy(user.private_username, private_username);
    strcpy(user.username, username);
    strcpy(user.password, password);
    fwrite(&user, sizeof(Users), 1, file);
    fclose(file);

    return 0;
}

UserSessions loginUser(char *private_username, char *password, char *IP) {
    //verifications
    UserSessions session;
    session.user_id = 0;

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
        fclose(forID);
    }

    //find user and identify the user
    FILE *userF = fopen(c_users, "rb");
    if(userF == NULL) {
        session.id = 0;
        return session;
    }
    Users user;
    while(fread(&user, sizeof(Users), 1, userF)) {
        if(!strcmp(user.private_username, private_username) && !strcmp(user.password, password)) {
            if(user.deleted) break;
            session.user_id = user.id;
            break;
        }
    }
    fclose(userF);
    if(!session.user_id) {
        session.id = 0;
        return session;
    }

    //defaults, create
    FILE *file = fopen(c_sessions, "ab");
    if(file == NULL) {
        session.id = 0;
        return session;
    }
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

Users getUserByLInt(long int search, IntUserSearch searchFor) {
    Users user;
    user.id = 0;
    FILE *find = fopen(c_users, "rb");
    if(find == NULL) return user;
    switch(searchFor) {
        case US_FOR_ID: {
            while(fread(&user, sizeof(Users), 1, find)) if(user.id == search) break;
            fclose(find);
            if(user.id != search || user.deleted) { Users userErr; userErr.id = 0; return userErr; }
            return user;
        };
        case US_FOR_PUBLIC_ID: {
            while(fread(&user, sizeof(Users), 1, find)) if(user.public_id == search) break;
            fclose(find);
            if(user.public_id != search || user.deleted) { Users userErr; userErr.id = 0; return userErr; }
            return user;
        };
        default: { fclose(find); return user; }
    }
}

Users getUserByString(char *search, StringUserSearch searchFor) {
    Users user;
    user.id = 0;
    FILE *find = fopen(c_users, "rb");
    if(find == NULL) return user;
    switch(searchFor) {
        case US_FOR_PRIVATE_USERNAME: {
            while(fread(&user, sizeof(Users), 1, find)) if(!strcmp(user.private_username, search)) break;
            fclose(find);
            if(strcmp(user.private_username, search) || user.deleted) { Users userErr; userErr.id = 0; return userErr; }
            return user;
        };
        case US_FOR_USERNAME: {
            while(fread(&user, sizeof(Users), 1, find)) if(!strcmp(user.username, search)) break;
            fclose(find);
            if(strcmp(user.username, search) || user.deleted) { Users userErr; userErr.id = 0; return userErr; }
            return user;
        };
        case US_FOR_PASSWORD: {
            while(fread(&user, sizeof(Users), 1, find)) if(!strcmp(user.password, search)) break;
            fclose(find);
            if(strcmp(user.password, search) || user.deleted) { Users userErr; userErr.id = 0; return userErr; }
            return user;
        };
        default: { fclose(find); return user; }
    }
}

UserSessions getUserSessionByToken(long int search) {
    UserSessions session;
    session.id = 0;
    FILE *find = fopen(c_sessions, "rb");
    if(find == NULL) return session;
    while(fread(&session, sizeof(UserSessions), 1, find)) if(session.token == search) break;
    fclose(find);
    if(session.token != search || session.deleted) { UserSessions sessionErr; sessionErr.id = 0; return sessionErr; }
    return session;
}