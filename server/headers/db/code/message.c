#include "../index.h"

int createMessage(char *message, long int grup_id, long int user_id) {
    GrupMembers member = getGrupMember(user_id, grup_id);
    if(!member.user_id) return 404;
    if(member.permissions%(p_write*10)/p_write == 1) return 403;
    Messages msg;
    // find last id
    Messages buffer;
    FILE *forID = fopen(c_grup_messages, "rb");
    if(forID == NULL) {
        msg.id = 1;
    } else {
        fseek(forID, 0, SEEK_END);
        long dim = ftell(forID);
        if (dim == 0) {
            msg.id = 1;
        } else {
            fseek(forID, -sizeof(Messages), SEEK_CUR);
            fread(&buffer, sizeof(Messages), 1, forID);
            msg.id = buffer.id + 1;
        }
        fclose(forID);
    }
    msg.deleted = 0;
    msg.user_id = member.user_id;
    msg.grup_id = member.grup_id;
    msg.public_id = generate_token();
    strcpy(msg.message, message);
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    msg.time_created = tm;

    FILE *file = fopen(c_grup_messages, "ab");
    if(file == NULL) return 1063;
    fwrite(&msg, sizeof(Messages), 1, file);
    fclose(file);

    return 0;
}

Messages getMessageByLInt(long int search, IntMessageSearch searchFor) {
    Messages msg;
    msg.id = 0;
    FILE *find = fopen(c_grup_messages, "rb");
    if(find == NULL) return msg;
    switch(searchFor) {
        case MSGS_FOR_ID: {
            while(fread(&msg, sizeof(Messages), 1, find)) if(msg.id == search) break;
            fclose(find);
            if(msg.id != search || msg.deleted) { Messages msgErr; msgErr.id = 0; return msgErr; }
            return msg;
        };
        case MSGS_FOR_PUBLIC_ID: {
            while(fread(&msg, sizeof(Messages), 1, find)) if(msg.public_id == search) break;
            fclose(find);
            if(msg.public_id != search || msg.deleted) { Messages msgErr; msgErr.id = 0; return msgErr; }
            return msg;
        };
        default: { fclose(find); return msg; }
    }
}

Messages *getMessagesByLInt(long int search, IntMessagesSearch searchFor) {
    int capacity = 1;
    Messages *msg = (Messages*)malloc(sizeof(Messages) * capacity);
    Messages buffer;
    buffer.id = 0;
    msg[0] = buffer;
    FILE *find = fopen(c_grup_messages, "rb");
    if(find == NULL) return msg;
    switch(searchFor) {
        case MSGS_FOR_USER_ID: {
            while(fread(&buffer, sizeof(Messages), 1, find)) {
                if(buffer.user_id == search && !buffer.deleted) {
                    msg[capacity-1] = buffer;
                    capacity++;
                    msg = (Messages*)realloc(msg, sizeof(Messages) * capacity);
                }
            }
            fclose(find);
            return msg;
        };
        case MSGS_FOR_GRUP_ID: {
            while(fread(&buffer, sizeof(Messages), 1, find)) {
                if(buffer.grup_id == search && !buffer.deleted) {
                    msg[capacity-1] = buffer;
                    capacity++;
                    msg = (Messages*)realloc(msg, sizeof(Messages) * capacity);
                }
            }
            fclose(find);
            return msg;
        };
        default: { fclose(find); return msg; }
    }
    // da doamne sa mearga.
}