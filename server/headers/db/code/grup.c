#include "../index.h"

int createGrup(char *name, int user_id) {
     //verifications
    Grups grup;

    // find last id
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
        fclose(forID);
    }

    // add the owner on GrupMembers file
    GrupMembers owner;
    FILE *addOwner = fopen(c_grup_members, "ab");
    if(addOwner == NULL) return 1064;
    owner.user_id = user_id;
    owner.grup_id = grup.id;
    owner.permissions = init+write+read+invite+kick+gPerm;
    owner.accept_by_user = 1;
    owner.deleted = 0;
    fwrite(&owner, sizeof(GrupMembers), 1, addOwner);
    fclose(addOwner);

    // defaults, create
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

int addMemberInGrup(long int grup_public_id, int user_id, long int target_public_id, int permissions) {
    //find grup
    Grups grup = getGrupByLId(grup_public_id, GS_FOR_PUBLIC_ID);
    if(!grup.id) return 404;
    
    // see user permissions
    int givePermissions = 0;
    int inv = 0;

    if(grup.owner == user_id) { givePermissions = 1; inv = 1; } else {
        GrupMembers grupMember = getGrupMember(user_id, grup.id);
        if(grupMember.user_id != user_id) return 404;
        givePermissions = grupMember.permissions%(gPerm*10)/gPerm == 2 ? 1:0;
        inv = grupMember.permissions%(invite*10)/invite == 2 ? 1:0;
    }
    if(!inv) return 403;
    if(!givePermissions) permissions = init;

    // target
    Users target = getUserByLInt(target_public_id, US_FOR_PUBLIC_ID);
    if(!target.id) return 404;

    GrupMembers grupMember = getGrupMember(target.id, grup.id);
    if(grupMember.grup_id == grup.id && grupMember.user_id == target.id) return 204;

    GrupMembers addTarget;
    FILE *addTargetF = fopen(c_grup_members, "ab");
    if(addTargetF == NULL) return 1064;
    addTarget.user_id = target.id;
    addTarget.grup_id = grup.id;
    addTarget.permissions = permissions;
    addTarget.accept_by_user = 0;
    addTarget.deleted = 0;
    fwrite(&addTarget, sizeof(GrupMembers), 1, addTargetF);
    fclose(addTargetF);
    return 0;
}

Grups getGrupByLId(long int search, IntGrupSearch searchFor) {
    Grups grup;
    grup.id = 0;
    FILE *find = fopen(c_grups, "rb");
    if(find == NULL) return grup;
    switch(searchFor) {
        case GS_FOR_ID: {
            while(fread(&grup, sizeof(Grups), 1, find)) if(grup.id == search) break;
            fclose(find);
            if(grup.id != search) { Grups grupErr; grupErr.id = 0; return grupErr; }
            return grup;
        }
        case GS_FOR_PUBLIC_ID: {
            while(fread(&grup, sizeof(Grups), 1, find)) if(grup.public_id == search) break;
            fclose(find);
            if(grup.public_id != search) { Grups grupErr; grupErr.id = 0; return grupErr; }
            return grup;
        }
        default: { Grups grupErr; grupErr.id = 0; return grupErr; }
    }
}

Grups *getGrupsByOwner(long int search) {
    int capacity = 1;
    Grups *grups = (Grups*)malloc(sizeof(Grups) * capacity);
    Grups buffer;
    buffer.id = 0;
    grups[0] = buffer;
    FILE *find = fopen(c_grups, "rb");
    if(find == NULL) return grups;
    while(fread(&buffer, sizeof(Grups), 1, find)) {
        if(buffer.owner == search) {
            grups[capacity-1] = buffer;
            capacity++;
            grups = (Grups*)realloc(grups, sizeof(Grups)*capacity);
        }
    }
    fclose(find);
    return grups;
    // da doamne sa mearga.
}

GrupMembers getGrupMember(long int user_id, long int grup_id) {
    GrupMembers member;
    member.user_id = 0;
    FILE *find = fopen(c_grup_members, "rb");
    if(find == NULL) return member;
    while(fread(&member, sizeof(GrupMembers), 1, find)) if(member.user_id == user_id && member.grup_id == grup_id) break;
    fclose(find);
    if(!(member.user_id == user_id && member.grup_id == grup_id)) {
        GrupMembers memberErr;
        memberErr.user_id = 0;
        return memberErr;
    }
    return member;
}

GrupMembers *getAllGrupMembers(long int grup_id) {
    int capacity = 1;
    GrupMembers *members = (GrupMembers*)malloc(sizeof(GrupMembers) * capacity);
    GrupMembers buffer;
    buffer.user_id = 0;
    members[0] = buffer;
    FILE *find = fopen(c_grup_members, "rb");
    if(find == NULL) return members;
    while(fread(&buffer, sizeof(GrupMembers), 1, find)) {
        if(buffer.grup_id == grup_id) {
            members[capacity-1] = buffer;
            capacity++;
            members = (GrupMembers*)realloc(members, sizeof(GrupMembers)*capacity);
        }
    }
    fclose(find);
    return members;
}