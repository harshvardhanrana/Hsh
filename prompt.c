#include "headers.h"

char ShellStartLocation[BUFFERLENGTH];

void SetShellStartLocation()
{
    if (getcwd(ShellStartLocation, BUFFERLENGTH) == NULL)
    {
        fprintf(stderr, "Current Location Invalid!\n");
        exit(EXIT_FAILURE);
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

void GetCurrentLocation(char *buffer, const int BufferLength)
{
    if (getcwd(buffer, BufferLength) == NULL)
    {
        fprintf(stderr, "Current Location Invalid!\n");
        exit(EXIT_FAILURE);
    }
    int RelativeIndex = RelativeLocationIndex(buffer, BufferLength);
    if (RelativeIndex == -1)
        return;
    buffer[0] = '~';
    for (int i = 1; i + RelativeIndex < BufferLength; i++)
    {
        buffer[i] = buffer[i + RelativeIndex];
        if (buffer[i] == '\0')
            return;
    }
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

    printf("<%s@%s:%s> ", User, SYSname, CurrentDir);
}
