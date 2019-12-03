#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ptrace.h>
#include <wait.h>
#include <sys/user.h>

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

void info_registers(pid_t pid){
    struct user_regs_struct regs;
    ptrace(PTRACE_GETREGS, pid, NULL, &regs);
    printf("rip 0x%016llx\n", regs.rip);
}

void wait_for_inferrior(pid_t pid){
    int status;
    if (waitpid(pid, &status, 0) == -1){
        fatal("ERROR: Couldn't wait for inferior.");
    }

    if (!WIFSTOPPED(status)){
        fatal("Unexpected waitpid status");
    }

}

void attach_pid(pid_t pid){

    ptrace(PTRACE_ATTACH, pid, NULL, NULL);
    wait_for_inferrior(pid);
    //interesting stuff
    info_registers(pid);

    ptrace(PTRACE_DETACH, pid, NULL, NULL);
}

void attach_exe(char * path){
    valid_path(path);

    char* argv[] = {path, NULL};
    pid_t pid;

    pid = fork();
    switch (pid){
        case 0: //inferrior
            ptrace(PT_TRACE_ME, 0, NULL, 0);
            if (execv(path, argv) == -1){
                fatal("execv failed");
            }

            break;
        case -1: //fork error
            break;
        default: //in the tracer
            wait_for_inferrior(pid);
            info_registers(pid);
            break;

    }
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
