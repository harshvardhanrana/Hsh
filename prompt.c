#include "headers.h"

char ShellStartLocation[BUFFERLENGTH];
int ForegroundProcTime;
char ForegroundProcMax[BUFFERLENGTH];

void SetShellStartLocation()
{
    if (getcwd(ShellStartLocation, BUFFERLENGTH) == NULL)
    {
        fprintf(stderr, "Current Location Invalid!\n");
        exit(EXIT_FAILURE);
    }
}

void AddForegroundProc(const char* ForegroundProc, int ExecTime) {
    if (ExecTime > ForegroundProcTime) {
        strcpy(ForegroundProcMax, ForegroundProc);
        ForegroundProcTime = ExecTime;
    }
}

void GetForegroundProc(char* Foreground, int BufferLength) {
    if (ForegroundProcTime == 0)
        Foreground[0] = '\0';
    else {
        snprintf(Foreground, BufferLength, " %s : %ds", ForegroundProcMax, ForegroundProcTime);
    }
}

int RelativeLocationIndex(const char *buffer, const int BufferLength)
{
    int index = 0;
    for (; index < BufferLength; index++)
    {
        if (buffer[index] != ShellStartLocation[index])
            break;
        if (buffer[index] == '\0' && ShellStartLocation[index] == '\0')
            return index;
    }
    if (ShellStartLocation[index] == '\0')
        return index;
    else
        return -1;
}

void MakeRelative(char *buffer, const int BufferLength) {
    int RelativeIndex = RelativeLocationIndex(buffer, BufferLength);
    if (RelativeIndex == -1)
        return;
    buffer[0] = '~';
    for (int i = 1; i + RelativeIndex-1 < BufferLength; i++)
    {
        buffer[i] = buffer[i + RelativeIndex-1];
        if (buffer[i] == '\0')
            return;
    }
}

void GetCurrentLocation(char *buffer, const int BufferLength)
{
    if (getcwd(buffer, BufferLength) == NULL)
    {
        fprintf(stderr, "Current Location Invalid!\n");
        exit(EXIT_FAILURE);
    }
    MakeRelative(buffer, BufferLength);
}

void GetHostName(char* Buffer, const int BufferLength) {
    if (gethostname(Buffer, BufferLength) == -1) {
        fprintf(stderr, "Unable to get host name!\n");
        exit(errno);
    }
}

char* GetUserName(void) {
    char* UserName = getlogin();
    if (UserName == NULL) {
        fprintf(stderr, "Unable to get username!\n");
        exit(errno);
    }
    return UserName;
}

void prompt()
{
    // Do not hardcode the prmopt
    char SYSname[BUFFERLENGTH];
    GetHostName(SYSname, BUFFERLENGTH);

    char *User = GetUserName();

    char CurrentDir[BUFFERLENGTH];
    GetCurrentLocation(CurrentDir, BUFFERLENGTH);

    char Foreground[BUFFERLENGTH];
    GetForegroundProc(Foreground, BUFFERLENGTH);
    ForegroundProcTime = 0;

    printf("<%s@%s:%s%s> ", User, SYSname, CurrentDir, Foreground);
}
