#include "../headers/headers.h"

int keepprinting(int timestamp) {
    int next = time(NULL);
    int prev = next - timestamp;
    float dummyf; char dummyc[50];
    while (1) {
        if (next - prev >= timestamp) {
            FILE* fptr = fopen("/proc/loadavg", "r");
            int latestpid;
            fscanf(fptr, "%f %f %f %s %d",&dummyf, &dummyf, &dummyf, dummyc, &latestpid);
            printf("%d\n",latestpid);
            prev = next;
        }
        next = time(NULL);
    }
}

int neonate(char** Args) {
    if (strcmp(Args[1],"-n") != 0) {
        PrintError("Invalid Argument: %s\n",Args[1]);
        return 1;
    }
    int timestamp = StringtoInt(Args[2]);
    if (timestamp <= 0) {
        PrintError("Timestamp can only be a positive integer!\n");
        return 1;
    }
    int pid = fork();
    if (pid == 0) {
        keepprinting(timestamp);
        exit(0);
    }
    else {
        enableRawMode();
        char inp;
        while (1) {
            int flag = 1;
            while(read(STDIN_FILENO, &inp, 1) == 1) {
                if (inp == 'x') {
                    kill(pid, SIGKILL);
                    flag = 0;
                    break;
                }
            }
            if (!flag)
                break;
        }
        disableRawMode();
    }
    return 0;
}