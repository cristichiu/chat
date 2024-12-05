#include "../index.h"

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
    owner.deleted = 0;
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

int addMemberInGrup(long int grup_public_id, int user_id, long int target_public_id, int permissions) {
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
    addTarget.deleted = 0;
    fwrite(&addTarget, sizeof(GrupMembers), 1, addTargetF);
    fclose(addTargetF);
    return 0;
}

Grups getGrupById(int search) {
    Grups grup;
    FILE *find = fopen(c_grups, "rb");
    if(find == NULL) return grup;
    while(fread(&grup, sizeof(Grups), 1, find)) if(grup.id == search) break;
    fclose(find);
    if(grup.id != search) { Grups grupErr; return grupErr; }
    return grup;
}

Grups *getGrupsByOwner(int search) {
    int capacity = 1;
    Grups *grups = (Grups*)malloc(sizeof(Grups) * capacity);
    Grups buffer;
    FILE *find = fopen(c_grups, "rb");
    if(find == NULL) return grups;
    while(fread(&buffer, sizeof(Grups), 1, find)) {
        if(buffer.owner == search) {
            grups[capacity-1] = buffer;
            capacity++;
            grups = (Grups*)realloc(grups, sizeof(Grups)*capacity);
        }
    }
    return grups;
    // da doamne sa mearga.
}