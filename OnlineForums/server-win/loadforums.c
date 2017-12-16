#include "server.h"
int LoadForums(){
    char tmp[BUFFLEN];
    int count = 0;

    FILE *fp;
    if ((fp=fopen("forums.txt", "r") )==NULL) {
        printf("%s %d","OpenErrorCode",1);
        return 1;
    }
    while(fgets(tmp, sizeof tmp, fp)) {
        char *newString = (char *)malloc(strlen(tmp));
        strcpy(newString,tmp);
        newString[strlen(newString) - 1] = '\0';
        forums[count++] = newString;
    };
    fclose(fp);
    return count;
}
