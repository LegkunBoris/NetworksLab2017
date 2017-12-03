#include "server.h"
//Чтение из файла
char* readfromfile(const char *path)
{
    FILE *fp;
    if ((fp=fopen(path, "r") )==NULL) {
        printf("Cannot open file.\n");
        return 1;
    }

    char *msg = "";
    char tmp[256];
    while(fgets(tmp, sizeof tmp, fp)) {
        msg = concatStrings(msg,tmp);
    };

    printf(msg);
    fclose(fp);

    return msg;

}
int readlinesfromfile(const char *path)
{
    FILE *fp;
    if ((fp=fopen(path, "r") )==NULL) {
        printf("Cannot open file.\n");
        return 1;
    }

    int lines = 0;
    char tmp[BUFFLEN];
    while(fgets(tmp, sizeof tmp, fp)) {
        lines++;
    };

    fclose(fp);

    return lines;

}
