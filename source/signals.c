#include "../headers/headers.h"

int Ping(char** Args) {
    if (Args[1] == NULL || Args[2] == NULL) {
        PrintError("Expected atleast 2 arguments!\n");
        return 1;
    }
    int pid = StringtoInt(Args[1]);
    int signum = StringtoInt(Args[2]);
    signum = (signum%32+32)%32;
    printf("Sent signal %d to process with pid %d\n", signum, pid);
    kill(pid, signum);
    return 0;
}

int bg(char* arg) {
    if (arg == NULL) {
        PrintError("Expected atleast 1 argument!\n");
        return 1;
    }
    int pid;
    if ((pid = StringtoInt(arg)) < 0) {
        PrintError("No such process found!\n");
        return 1;
    }
    kill(pid, 18);
    return 0;
}

int fg(char* arg) {
    if (arg == NULL) {
        PrintError("Expected atleast 1 argument!\n");
        return 1;
    }
    int pid;
    if ((pid = StringtoInt(arg)) < 0) {
        PrintError("No such process found!\n");
        return 1;
    }
    kill(pid, 18);
    RemoveProcWithPid(pid);
    int y;
    waitpid(pid, &y, 0);
    return 0;
}

extern int CHILDPID;

void interruptfg(int dummy) 
{
    if (CHILDPID != 0)
        kill(CHILDPID, SIGKILL);
}