#include "../headers/headers.h"

extern int ExitFlag;

int main()
{
    signal(SIGINT, interruptfg);
    SetShellStartLocation();
    ReadFromFile();
    // Keep accepting commands
    while (1)
    {
        ExitFlag = 0;
        // Print appropriate prompt with username, systemname and directory before accepting input
        prompt();
        char input[4096];
        fgets(input, 4096, stdin);
        input[strcspn(input, "\n")] = '\0';
        RemoveBackgroundBuffers();
        SplitStrings(input, 4096, 0);
        if (ExitFlag)
            exit(0);
    }
}
