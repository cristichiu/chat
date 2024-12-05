#ifndef DB_GRUP_H
#define DB_GRUP_H

#include "index.h"

typedef enum IntGrupSearch { S_FOR_ID, S_FOR_OWN } IntGrupSearch;

int createGrup(char *name, int user_id);
int addMemberInGrup(long int grup_public_id, int user_id, long int target_public_id, int permissions);


#endif