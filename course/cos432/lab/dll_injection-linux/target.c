#include <stdio.h>
#include <string.h>
//#include <>
#include "auth.h"


int main(int argc, char *argv[]){

    char username[MAX_STRING_LEN];
    char password[MAX_STRING_LEN];

    printf("Username: ");
    scanf("%s", username);

    printf("Password: ");
    scanf("%s", password);

    if(authenticate(username, password)){
        printf("you're in");
    }
    else {
        printf("you did it wrong");

    }


    return 0;
}