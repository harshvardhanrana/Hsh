#include "../headers/headers.h"

int BackgroundProcCount = 0;

struct BackgroundProc
{
    char CommandName[1024];
    int Pid;
    struct BackgroundProc* next;
};

struct BackgroundProc* Last = NULL;
struct BackgroundProc* First = NULL;

void AddBackgroundProcess(char *CommandName, int Pid)
{
    if (First == NULL) {
        First = malloc(sizeof(struct BackgroundProc));
        Last = First;
    }
    assert(First != NULL);
    Last->next = malloc(sizeof(struct BackgroundProc));
    Last = Last->next;
    Last->Pid = Pid;
    strcpy(Last->CommandName, CommandName);
    Last->next = NULL;
    BackgroundProcCount++;
}

void RemoveProc(struct BackgroundProc* Childprev) {
    struct BackgroundProc* temp = Childprev->next;
    if (Last == temp)
        Last = Childprev;
    if (temp != NULL) {
        Childprev->next = temp->next;
        free(temp);
    }
    BackgroundProcCount--;
}

struct BackgroundProc* GetProc(int pid) {
    struct BackgroundProc* trav = First;
    while (trav->next != NULL) {
        if (trav->next->Pid == pid) {
            return trav;
        }
        trav = trav->next;
    }
    return NULL;
}

void RemoveProcWithPid(int pid, char* CommandName) {
    struct BackgroundProc* prev = GetProc(pid);
    if (prev != NULL) {
        strcpy(CommandName, prev->next->CommandName);
        RemoveProc(prev);
    }
}

void KillAllProcs() {
    if (First == NULL)
        return;
    struct BackgroundProc* trav = First;
    while (trav->next != NULL) {
        trav = trav->next;
        kill(trav->Pid, SIGKILL);
    }
}

void RemoveBackgroundBuffers() {
    if (First == NULL)
        return;
    struct BackgroundProc* trav = First;
    int pid;
    int status;
    while ((pid = waitpid(-1, &status, WNOHANG | WUNTRACED)) > 0) {
        struct BackgroundProc* ChildPrev = GetProc(pid);
        if (ChildPrev == NULL) {
            PrintError("Background Process not found with %d\n", pid);
            return;
        }
        if(WIFEXITED(status) || WIFSTOPPED(status)){
        	printf("%s exited normally (%d)\n", ChildPrev->next->CommandName, ChildPrev->next->Pid);
            RemoveProc(ChildPrev);
        }
        else if (WIFSTOPPED(status)) {
            printf("%s suspended normally (%d)\n", ChildPrev->next->CommandName, ChildPrev->next->Pid);
        } 
        else {
            printf("%s did not exit normally (%d)\n", ChildPrev->next->CommandName, ChildPrev->next->Pid);
            RemoveProc(ChildPrev);
        }
    }
}

int Activities() {
    if (First == NULL || First->next == NULL) {
        printf("No process found!\n");
        return 0;
    }
    struct BackgroundProc* trav = First;
    int status;
    while (trav->next != NULL) {
        trav = trav->next;
        char stat_path[BUFFERLENGTH];
        snprintf(stat_path, sizeof stat_path, "/proc/%d/stat", trav->Pid);
        FILE* fptr = fopen(stat_path, "r");
        if (fptr == NULL) {
            PrintError("Invalid Process ID: %d\n", trav->Pid);
            return -1;
        }
        int dummy; char dummys[100];
        char state;
        fscanf(fptr, "%d %s %c", &dummy, dummys, &state);
        printf("%d : %s - %s\n", trav->Pid, trav->CommandName, (state == 'Z' || state == 'T')? "Stopped" : "Running");
    }
    return 0;
}
