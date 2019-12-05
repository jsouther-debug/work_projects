#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <wait.h>
#include <sys/personality.h>
#include <signal.h>
#include <stdint.h>


#define ELF_MAGIC_NUMBER "\x7F\x45\x4C\x46"

#define REG_RIP 1 // 64 bit instruction pointer
#define REG_ECX 2 // 32 bit version of rcx

// base in IDA: 0x555555554000
// base in Linux w/o ASLR: 0x555555554000
// diff: 0xFB03A81000

// BP in IDA: 000055C7A6FD53B2 breakpoint for cheat
// BP in IDA for after the program actually is waiting for user input:0x00005592DB8E421E?
#define BREAK_POINT_ADDRESS 0x0000559D92466AC0 //right after it gets an input from the user


void fatal(const char * msg) {

    fprintf(stderr, "ERROR: %s\n", msg);
    exit(-1);
}


void validate_exe_path(const char * path) {

    int fd;
    char magic_number[4];

    fd = open(path, O_RDONLY);
    if (fd == -1) fatal("Invalid path");

    read(fd, magic_number, 4);
    if (memcmp(magic_number, ELF_MAGIC_NUMBER, 4) != 0) {
        fatal("Not an ELF file.");
    }

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


uint64_t get_register(pid_t pid, int reg) {

    uint64_t value;
    struct user_regs_struct regs;

    ptrace(PTRACE_GETREGS, pid, NULL, &regs);

    switch (reg) {

        case REG_RIP:
            value = regs.rip;
            break;

        default:
            value = 0;
            break;

    }

    return value;

}


uint64_t set_register(pid_t pid, int reg, uint64_t new_value) {

    uint64_t former_value;
    struct user_regs_struct regs;

    ptrace(PTRACE_GETREGS, pid, NULL, &regs);

    switch (reg) {

        case REG_RIP:
            former_value = regs.rip;
            regs.rip = new_value;
            break;

        default:
            former_value = 0;
            break;

    }

    ptrace(PTRACE_SETREGS, pid, NULL, &regs);
    return former_value;
}


uint32_t get_register32(pid_t pid, int reg) {

    uint32_t value;
    struct user_regs_struct regs;

    ptrace(PTRACE_GETREGS, pid, NULL, &regs);

    switch (reg) {

        case REG_ECX: // note: rcx is the 64 bit register whereas ecx is the lower 32 bits of rcx
            value = (uint32_t) regs.rcx & 0x00000000FFFFFFFF;
            break;

        default:
            value = 0;
            break;

    }

    return value;

}


uint32_t set_register32(pid_t pid, int reg, uint32_t new_value) {

    struct user_regs_struct regs;
    uint32_t former_value;

    ptrace(PTRACE_GETREGS, pid, NULL, &regs);

    switch (reg) {

        case REG_ECX: // note: rcx is the 64 bit register whereas ecx is the lower 32 bits of rcx
            former_value = (uint32_t) regs.rcx & 0x00000000FFFFFFFF;
            regs.rcx = (regs.rcx & 0xFFFFFFFF00000000) | new_value;
            break;

        default:
            former_value = 0;
            break;

    }

    ptrace(PTRACE_SETREGS, pid, NULL, &regs);
    return former_value;
}




uint64_t poke_int3(pid_t pid, uint64_t location) {

    uint64_t data, save, int3, data_with_int3;

    data = ptrace(PTRACE_PEEKDATA, pid, location, NULL);
    save = data & 0xfful;
    int3 = 0xcc;
    data_with_int3 = ((data & ~0xfful) | int3);

    if (ptrace(PTRACE_POKEDATA, pid, location, data_with_int3) == -1) {
        fatal("Poking int3 failed");
    }

    return save;

}


int unpoke_int3(pid_t pid, uint64_t location, uint64_t replaced_data) {

    uint64_t data;
    int status = 0;

    data = ptrace(PTRACE_PEEKDATA, pid, location, NULL);
    data = ((data & ~0xfful) | replaced_data);
    status = ptrace(PTRACE_POKEDATA, pid, location, data);

    return status;
}


void single_step(pid_t pid) {

    int status;

    alarm_signal_set(pid, 0);
    ptrace(PTRACE_SINGLESTEP, pid, (char *) 1, 0);

    if (waitpid(pid, &status, 0) == -1) {
        fatal("Inferior changed state in an unpredicted way during single step!?");
    }

    alarm_signal_set(pid, 1);
}


void wait_for_inferior(pid_t pid) {

    int status;
    uint64_t save = 0, ip;
    int first_stop = 1;
    uint32_t save32reg = 0;

    while(1) {

        if ( waitpid(pid, &status, 0) == -1) {
            fatal("Error: Couldn't wait for inferior.");
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

                fatal("Segfault in xdeminuer");

            } else if (WSTOPSIG(status) == SIGTRAP) {

                printf("It's a trap!\n");

                if (first_stop) { // initial trap
                    first_stop = 0;

                    save = poke_int3(pid, BREAK_POINT_ADDRESS);

                } else {

                    ip = get_register(pid, REG_RIP);
                    if (ip - 1 == BREAK_POINT_ADDRESS) {
                        unpoke_int3(pid, BREAK_POINT_ADDRESS, save);
                        set_register(pid, REG_RIP, BREAK_POINT_ADDRESS);

                        save32reg = set_register32(pid, REG_ECX, 12);

                        single_step(pid);

                        set_register32(pid, REG_ECX, save32reg);

                        save = poke_int3(pid, BREAK_POINT_ADDRESS);

                    }


                }


            } else {
                printf("Warning: unhandled stop signal.\n");
            }
        }

        ptrace(PTRACE_CONT, pid, (char *) 1, 0);

    }

}


void attach_exe(char * path, char ** argv) {

    pid_t pid;

    validate_exe_path(path);

    pid = fork();
    switch (pid) {

        case 0: // inferior
            ptrace(PT_TRACE_ME, 0, NULL, 0);
            personality(ADDR_NO_RANDOMIZE);
            if (execv(path, argv) == -1) {
                fatal("Execv failed.");
            }

            break;

        case -1: // error
            break;

        default: // tracer
            wait_for_inferior(pid);
            break;

    }

}





int main(int argc, char *argv[]) {

    if (argc < 2) {
        fatal("Requires xdemineur with optional arguments.");
    }

    attach_exe(argv[1], argv+1);

    return 0;
}
