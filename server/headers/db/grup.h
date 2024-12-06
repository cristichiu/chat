#ifndef DB_GRUP_H
#define DB_GRUP_H

#include "index.h"

typedef enum IntGrupSearch { GS_FOR_ID, GS_FOR_PUBLIC_ID } IntGrupSearch;
typedef enum IntGrupMemberSearch { MS_FOR_USER_ID, MS_FOR_GRUP_ID } IntGrupMemberSearch;

int createGrup(char *name, int user_id);
int addMemberInGrup(long int grup_public_id, int user_id, long int target_public_id, int permissions);

Grups getGrupByLId(long int search, IntGrupSearch searchFor);
Grups *getGrupsByOwner(long int search);
GrupMembers getGrupMember(long int user_id, long int grup_id);
GrupMembers *getAllGrupMembers(long int grup_id);
GrupMembers *getAllMyGrups(long int user_id);
int acceptInvitation(long int user_id, long int grup_id);
int kickFromGrup(long int target_id, long int user_id, long int grup_id);

#endif