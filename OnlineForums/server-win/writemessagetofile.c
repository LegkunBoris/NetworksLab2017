#include "server.h"
//Запись сообщения в файл
void WriteMessageToFile(char *path,char *data)
{
    FILE *f = fopen(path,"a");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        return;
    }
    fprintf(f, data);
    fprintf(f, "\n");
    fclose(f);
}
