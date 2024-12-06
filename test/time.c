#include <stdio.h>
#include <time.h>
#include <string.h>

int main() {
  char test[100] = "acesta:&test";
  char *token = strtok(test, ":&");
  printf("%s\n", token);
  token = strtok(NULL, ":&");
  printf("%s\n", token);
  return 0;
}