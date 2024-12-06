#ifndef DB_UTILITIES_H
#define DB_UTILITIES_H

int verifyDB();
int verify(char *filePath);
typedef enum ShowTableOption { SHOW_GRUPS, SHOW_USERS, SHOW_MESSAGES, SHOW_SESSIONS, SHOW_GRUP_MEMBERS } ShowTableOption;
void showTable(ShowTableOption option);

#endif