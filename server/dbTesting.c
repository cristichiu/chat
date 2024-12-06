#include "./headers/db/index.h"

int main() {
    srand(time(NULL));
    long int token = 4452924401234556; // user 1
    //token = 1104023631938694; // user 2
    //token = 9288424929268349; // user 3
    //token = 0;

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
            printf("Select option:\n1: Vezi username-ul tau\n2: Creaza grup\n3: Vezi toate grupurile tale (owner)\n4: Adauga un utilizator la un grup\n5: Vezi toti utilizatorii din grup\n6: Scrie un mesaj\n7: Vezi toate mesajele\n8: Vezi toate grupurile tale (member)\n9: accepta invitatia intr-un grup\n10: kick\n");
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
                    int count = 0;
                    while(allGrups[count].id) {
                        printf("%ld - %s\n", allGrups[count].public_id, allGrups[count].name);
                        count++;
                    }
                    break;
                }
                case 4: {
                    long int grup_public_id;
                    long int user_public_id;
                    int permissions = p_init+p_read+p_write;
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
                    int count = 0;
                    while(allMembers[count].user_id) {
                        Users user = getUserByLInt(allMembers[count].user_id, US_FOR_ID);
                        if(user.id) {
                            printf("%ld - %s - %d (%d)\n", user.public_id, user.username, allMembers[count].permissions, allMembers[count].accept_by_user);
                        }
                        count++;
                    }
                    break;
                }
                case 6: {
                    long int grup_public_id;
                    char message[1024];
                    printf("grup_public_id: "); scanf("%ld\n", &grup_public_id);
                    fgets(message, 1024, stdin);
                    if(message[strlen(message)-1] == '\n') message[strlen(message)-1] = '\0';

                    Grups grup = getGrupByLId(grup_public_id, GS_FOR_PUBLIC_ID);
                    if(!grup.id) {
                        printf("Ceva nu a mers bine, nu am gasit grupul.");
                        break;
                    }

                    if(createMessage(message, grup.id, user.id)) printf("Ceva nu a mers bine"); else printf("mesaj creat cu succes");
                    break;
                }
                case 7: {
                    long int grup_public_id;
                    printf("grup_public_id: "); scanf("%ld", &grup_public_id);

                    Grups grup = getGrupByLId(grup_public_id, GS_FOR_PUBLIC_ID);
                    if(!grup.id) {
                        printf("Ceva nu a mers bine, nu am gasit grupul.");
                        break;
                    }

                    GrupMembers verify = getGrupMember(user.id, grup.id);
                    if(!verify.user_id || verify.permissions%(p_read*10)/p_read == 1 || !verify.accept_by_user || verify.deleted) {
                        printf("Nu ai acces sa vezi ce este in acest grup");
                        break;
                    }

                    Messages *msgs = getMessagesByLInt(grup.id, MSGS_FOR_GRUP_ID);
                    int count = 0;
                    while(msgs[count].id) {
                        Users user = getUserByLInt(msgs[count].user_id, US_FOR_ID);
                        if(user.id) {
                            printf("%s: %d-%02d-%02d | %02d:%02d:%02d -> %s\n", user.username, msgs[count].time_created.tm_year + 1900, msgs[count].time_created.tm_mon + 1, msgs[count].time_created.tm_mday, msgs[count].time_created.tm_hour, msgs[count].time_created.tm_min, msgs[count].time_created.tm_sec, msgs[count].message);
                        }
                        count++;
                    }
                    break;
                }
                case 8: {
                    GrupMembers *allMyGrups = getAllMyGrups(user.id);
                    int count = 0;
                    while(allMyGrups[count].user_id) {
                        Grups grup = getGrupByLId(allMyGrups[count].grup_id, GS_FOR_ID);
                        if(grup.id) {
                            printf("%ld - %s - %d (%d)\n", grup.public_id, grup.name, allMyGrups[count].permissions, allMyGrups[count].accept_by_user);
                        }
                        count++;
                    }
                    break;
                }
                case 9: {
                    long int grup_public_id;
                    printf("grup_public_id: "); scanf("%ld", &grup_public_id);
                    Grups grup = getGrupByLId(grup_public_id, GS_FOR_PUBLIC_ID);
                    if(!grup.id) {
                        printf("Nu am gasit grupul");
                        break;
                    }
                    if(acceptInvitation(user.id, grup.id)) printf("ceva nu a mers bine"); else printf("Ai acceptat invitatia cu succes");
                    break;
                }
                case 10: {
                    long int target_public_id;
                    long int grup_public_id;
                    printf("target_public_id: "); scanf("%ld", &target_public_id);
                    printf("grup_public_id: "); scanf("%ld", &grup_public_id);

                    Users target = getUserByLInt(target_public_id, US_FOR_PUBLIC_ID);
                    if(!target.id) {
                        printf("Nu am gasit target-ul");
                        break;
                    }
                    if(target.id == user.id) {
                        printf("nu poti sa-ti dai kick singur!");
                        break;
                    }
                    Grups grup = getGrupByLId(grup_public_id, GS_FOR_PUBLIC_ID);
                    if(!grup.id) {
                        printf("Nu am gasit grupul.");
                        break;
                    }
                    if(kickFromGrup(target.id, user.id, grup.id)) printf("Ceva nu a mers bine la kick"); else printf("Am dat utilizatorul afara cu succes");
                }
            }
        }
    }
    return 0;
}