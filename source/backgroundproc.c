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
    Childprev->next = temp->next;
    free(temp);
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

void RemoveProcWithPid(int pid) {
    struct BackgroundProc* prev = GetProc(pid);
    RemoveProc(prev);
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
        if(WIFEXITED(status)){
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
    if (First == NULL) {
        printf("No process found!\n");
        return 0;
    }
    struct BackgroundProc* trav = First;
    int status;
    while (trav->next != NULL) {
        trav = trav->next;
        waitpid(trav->Pid, &status, WNOHANG | WUNTRACED);
        printf("%d : %s - %s\n", trav->Pid, trav->CommandName, WIFSTOPPED(status) ? "Stopped" : "Running");
    }
}
