#include "../headers/headers.h"

void GetProcLocation(int procid, char Buffer[], int BufferLength) {
    char path_buf[BUFFERLENGTH];
    int path_len = snprintf(path_buf, sizeof path_buf, "/proc/%d/exe", procid);
    int byteswritten = readlink(path_buf, Buffer, BUFFERLENGTH);
    if (byteswritten == -1) {
        strcpy(Buffer, "Not Found!");
        return;
    }
    Buffer[byteswritten] = '\0';
    MakeRelative(Buffer, BufferLength);
}

int Proclore(char* Argument) {
    int procid;
    if (Argument == NULL) {
        procid = getpid();
    }
    else {
        procid = 0;
        for (int i=0; Argument[i] != '\0'; i++) {
            procid *= 10;
            procid += (int)(Argument[i] - '0');
        }
    }
    
    char stat_path[BUFFERLENGTH];
    snprintf(stat_path, sizeof stat_path, "/proc/%d/stat", procid);

    FILE* fptr = fopen(stat_path, "r");
    if (fptr == NULL) {
        PrintError("Invalid Process ID: %d\n", procid);
        return -1;
    }

    printf("pid : %d\n", procid);

    int dummy1;
    char dummy2[BUFFERLENGTH];
    char state; 
    int grpid;
    unsigned long virtualmemory;
    fscanf(fptr, "%d %s %c %d %d %d %d %d %d %d", &dummy1, dummy2, &state, &dummy1, &grpid, &dummy1, &dummy1, &dummy1, &dummy1, &dummy1);
    fscanf(fptr, "%d %d %d %d %d %d %d %d %d %d", &dummy1, &dummy1, &dummy1, &dummy1, &dummy1, &dummy1, &dummy1, &dummy1, &dummy1, &dummy1);
    fscanf(fptr, "%d %d %lu", &dummy1, &dummy1, &virtualmemory);
    
    printf("process status : %c", state);
    if (getpid() == procid) {
        printf("+");
    }
    printf("\n");
    printf("Process Group : %d\n", grpid);
    printf("Virtual memory : %lu\n", virtualmemory);

    char exe_path[BUFFERLENGTH];
    GetProcLocation(procid, exe_path, BUFFERLENGTH);

    
    printf("executable Path : %s\n", exe_path);
    return 0;
}