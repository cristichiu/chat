#include "./headers/db/index.h"

int main() {
    srand(time(NULL));
    long int token = 4452924401234556; // user 1
    //token = 1104023631938694; // user 2
    //token = 9288424929268349; // user 3

    while(1) {
        if(!token) {
            int option;
            printf("Select option:\n1: register\n2: login\n");
            scanf("%d", &option);
            switch(option) {
                case 1: {
                    char username[64];
                    char private_username[64];
                    char password[64];
                    printf("username: "); scanf("%s", username);
                    printf("private_username: "); scanf("%s", private_username);
                    printf("password: "); scanf("%s", password);
                    if(createUser(private_username, username, password)) {
                        printf("User-ul nu a fost creat.\n");
                    } else printf("User creat cu succes\n");
                    break;
                }
                case 2: {
                    char private_username[64];
                    char password[64];
                    printf("private_username: "); scanf("%s", private_username);
                    printf("password: "); scanf("%s", password);
                    UserSessions login = loginUser(private_username, password, "todo");
                    if(!login.id) {
                        printf("Ceva nu a mers bine la logare\n");
                    } else {
                        token = login.token;
                        printf("Logat cu succes\n");
                    }
                    break;
                }
            }
        } else {
            int option;
            printf("Select option:\n1: Vezi username-ul tau\n2: Creaza grup\n3: Vezi toate grupurile tale\n4: Adauga un utilizator la un grup\n5: Vezi toti utilizatorii din grup\n");
            scanf("%d", &option);
            UserSessions session = getUserSessionByToken(token);
            if(!session.id) { printf("Ceva nu a mers bine, nu am putut gasi sesiunea ta!\n"); break; }
            Users user = getUserByLInt(session.user_id, US_FOR_ID);
            if(!user.id) { printf("Ceva nu a mers bine nu am putut gasi userul dupa sesiune\n"); break; }
            switch(option) {
                case 1: { printf("Esti cumva asta tu?: %ld - %ld - %s\n", user.public_id, session.token, user.username); break; }
                case 2: {
                    char name[64];
                    printf("Name: "); scanf("%s", name);
                    if(createGrup(name, user.id)) printf("Nu s-a creat grupul\n"); else printf("Grup creat cu succes\n");
                    break;
                }
                case 3: {
                    Grups *allGrups = getGrupsByOwner(user.id);
                    int count;
                    while(allGrups[count].id) {
                        printf("%ld - %s\n", allGrups[count].public_id, allGrups[count].name);
                        count++;
                    }
                    break;
                }
                case 4: {
                    long int grup_public_id;
                    long int user_public_id;
                    int permissions = init+read+write;
                    printf("grup_public_id: "); scanf("%ld", &grup_public_id);
                    printf("user_public_id: "); scanf("%ld", &user_public_id);
                    if(addMemberInGrup(grup_public_id, user.id, user_public_id, permissions)) printf("Ceva nu a mers cum trebuie\n"); else printf("User adaugat cu succes\n");
                    break;
                }
                case 5: {
                    long int grup_public_id;
                    printf("grup_public_id: "); scanf("%ld", &grup_public_id);
                    Grups grup = getGrupByLId(grup_public_id, GS_FOR_PUBLIC_ID);
                    if(!grup.id) { printf("Grup negasit."); break; }
                    GrupMembers *allMembers = getAllGrupMembers(grup.id);
                    int count;
                    while(allMembers[count].user_id) {
                        Users user = getUserByLInt(allMembers[count].user_id, US_FOR_ID);
                        if(user.id) {
                            printf("%s - %d (%d)\n", user.username, allMembers[count].permissions, allMembers[count].accept_by_user);
                        }
                        count++;
                    }
                    break;
                }
            }
        }
    }
    return 0;
}