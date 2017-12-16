#include "server.h"
//Чтение из файла
char* ReadFromFile(const char *path)
{
    char *msg = "";
    char tmp[BUFFLEN];
    FILE *fp;

    if ((fp=fopen(path, "r") )==NULL) {
        printf("Cannot open file.\n");
        return 1;
    }
    while(fgets(tmp, sizeof tmp, fp)) {
        msg = ConcatStrings(msg,tmp);
    };

    printf(msg);
    fclose(fp);
    return msg;

}
int ReadLinesFromFile(const char *path)
{
    int lines = 0;
    char tmp[BUFFLEN];
    FILE *fp;
    if ((fp=fopen(path, "r") )==NULL) {
        printf("Cannot open file.\n");
        return 1;
    }

    while(fgets(tmp, sizeof tmp, fp)) {
        lines++;
    };
    fclose(fp);
    return lines;

}
