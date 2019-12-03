#include <stdio.h>
#include <string.h>
#include "auth.h"

int authenticate(char * username, char * password){



    if (strncmp(username, "whynot", MAX_STRING_LEN) == 0){
        if (strncmp(username, "notwhy", MAX_STRING_LEN) == 0){
            return 1;
        }
    }

    return 0;
}
