#include "client.h"
char* concatStrings(char *dist, char *source){
    void *newString = malloc(strlen(dist) + strlen(source));
    strcpy(newString,dist);
    strcat(newString,source);
    return newString;
}
