#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define ELF_MAGIC_NUMBER "\x7f\x45\x4C\x46"

void fatal(const char * msg){
    fprintf(stderr, "ERROR: %s\n", msg);
    exit(-1);
}

void valid_path(const char * path){
    int fd;
    char magic_number[4];
    fd = open(path, O_RDONLY);
    if (fd == -1) fatal("invalid path");

    read(fd, magic_number, 4);
    if (memcmp(magic_number, ELF_MAGIC_NUMBER, 4) != 0) fatal("not an elf file");
}

void attach_exe(const char * path){
    valid_path(path);
}


int main(int argc, char *argv[]) {

    pid_t pid;

    if (argc != 2){
        fatal("Requires PID or ELF file");
    }

    pid = strtol(argv[1], NULL, 0);
    if(pid != 0){
        attach_pid(pid);
    }
    else {
        attach_exe(argv[1]);
    }

    return 0;
}
