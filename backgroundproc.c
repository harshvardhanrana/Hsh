#include "headers.h"

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
}

void RemoveBackgroundBuffers() {
    if (First == NULL)
        return;
    struct BackgroundProc* trav = First;
    while (trav->next != NULL) {
        struct BackgroundProc* temp = trav->next;
        int status;
        char path_buf[BUFFERLENGTH];
        char Buffer[BUFFERLENGTH];
        int path_len = snprintf(path_buf, sizeof path_buf, "/proc/%d/exe", trav->next->Pid);
        int byteswritten = readlink(path_buf, Buffer, BUFFERLENGTH);
        if (byteswritten == -1) {
            printf("%s exited normally (%d)\n", trav->next->CommandName, trav->next->Pid);
            trav->next = trav->next->next;
            free(temp);
        }
        else {
            trav = trav->next;
        }
    }
    Last = trav;
}