#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ptrace.h>
#include <wait.h>
#include <sys/user.h>
#include <sys/personality.h>

#define ELF_MAGIC_NUMBER "\x7f\x45\x4C\x46"

//base in ida: 0x559A2A367000
//base in linux without aslr: 0x555555554000
//diff = base in ida - base in linux = 44D4E13000

//breakpoint in ida: 0000559A2A36F01A

#define BREAK_POINT_ADDRESS 0x55555555C01A

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

void alarm_signal_set(pid_t pid, int on) {
    sigset_t sig_mask_set;
    sigemptyset(&sig_mask_set);
    // Manpage says sizeof(siget_t) but it needs size 8 to work?!
    ptrace(PTRACE_GETSIGMASK, pid, sizeof(int64_t), &sig_mask_set);
    if (on) {
        sigdelset(&sig_mask_set, SIGALRM);
    } else {
        sigaddset(&sig_mask_set, SIGALRM);
    }
    ptrace(PTRACE_SETSIGMASK, pid, sizeof(int64_t), &sig_mask_set);
}

void wait_for_inferrior(pid_t pid) {
    int status;
    int first_stop = 1;


    while (1) {

        if (waitpid(pid, &status, 0) == -1) {
            fatal("ERROR: Couldn't wait for inferior.");
        }

        if (WIFEXITED(status)) {
            printf("xdemineur exited.\n");
            return;
        } else if (WIFSTOPPED(status)) {
            if (WSTOPSIG(status) == SIGALRM) {
                printf(".");
                fflush(stdout);
                ptrace(PTRACE_CONT, pid, (char *) 1, SIGALRM);
                continue;
            } else if (WSTOPSIG(status) == SIGSEGV) {
                fatal("Segfault xdemineur");
            } else if (WSTOPSIG(status) == SIGTRAP) {
                printf("it's a trap");
                if (first_stop){
                    first_stop = 0;
                    alarm_signal_set(pid, 0);
                }else{
                    //create breakpoint
                }
            } else {
                printf("Warning: unhandled stop signal.\n");
            }

        }
        ptrace(PTRACE_CONT, pid, (char *) 1, 0);
    }
}

void attach_exe(char * path, char ** argv){
    valid_path(path);

    pid_t pid;

    pid = fork();
    switch (pid){
        case 0: //inferrior
            ptrace(PT_TRACE_ME, 0, NULL, 0);
            personality(ADDR_NO_RANDOMIZE);
            if (execv(path, argv) == -1){
                fatal("execv failed");
            }

            break;
        case -1: //fork error
            break;
        default: //in the tracer
            wait_for_inferrior(pid);
            break;

    }
}


int main(int argc, char *argv[]) {

    if (argc < 2){
        fatal("Requires xdemineur with optional args");
    }

    attach_exe(argv[1], argv+1);

    return 0;
}
