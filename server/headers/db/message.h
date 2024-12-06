#ifndef DB_MESSAGE_H
#define DB_MESSAGE_H

#include "index.h"

typedef enum IntMessageSearch { MSGS_FOR_ID, MSGS_FOR_PUBLIC_ID } IntMessageSearch; 
typedef enum IntMessagesSearch { MSGS_FOR_GRUP_ID, MSGS_FOR_USER_ID } IntMessagesSearch;

int createMessage(char *message, long int grup_id, long int user_id);
Messages getMessageByLInt(long int search, IntMessageSearch searchFor);
Messages *getMessagesByLInt(long int search, IntMessagesSearch searchFor);

#endif