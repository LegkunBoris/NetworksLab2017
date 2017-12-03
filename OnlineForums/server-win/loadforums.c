#include "server.h"

int loadForums(){
    int count = 0;

    FILE *fp;
    if ((fp=fopen("forums.txt", "r") )==NULL) {
        printf("%s %d","OpenErrorCode",1);
        return 1;
    }

    char tmp[BUFFLEN];

    int num_chars = 0;

    int ch;

    while ((ch = fgetc(fp))!= EOF){
        if(ch != '\n')
            tmp[num_chars++] = ch;
        else{
            tmp[num_chars] = '\0';

            char *newString = (char *)malloc(num_chars);

            strcpy(newString,tmp);

            newString[strlen(newString)] = '\0';

            forums[count] = newString;

            printf("forums[%d] [%s] added\n",count,forums[count]);

            memset(tmp,0,strlen(tmp));

            num_chars = 0;

            count++;
        }
    }
    fclose(fp);
    return count;
}
