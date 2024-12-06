#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

int main() {
  char test[100] = "acesta:&test:&idk";
  char *token = strtok(test, ":&");
  printf("%s\n", token);
  token = strtok(NULL, ":&");
  printf("%s\n", token);
  token = strtok(NULL, ":&");
  printf("%s\n", token);
  
  char **array = (char**)malloc(3*sizeof(char*));
  for(int i=0; i<3; i++) {
    array[i] = (char*)malloc(64*sizeof(char));
  }
  array[0] = "test";
  printf("%s", array[0]);

  return 0;
}