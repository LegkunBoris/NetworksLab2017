#include "server.h"

int decodeString(char *str) {
    if(strcmp(str, "help") == 0)
        return HELP;
    else if(strcmp(str, "quit") == 0)
        return QUIT;
    else if(strcmp(str, "forums") == 0)
        return FORUMS;
    else if(strcmp(str, "open") == 0)
        return OPEN;
    else if(strcmp(str, "name") == 0)
        return NAME;
    else if(strcmp(str, "create") == 0)
        return CREATE;
    else
        return -1;
}
