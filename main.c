#include "headers.h"

int main()
{
    SetShellStartLocation();
    // Keep accepting commands
    while (1)
    {
        // Print appropriate prompt with username, systemname and directory before accepting input
        prompt();
        char input[4096];
        fgets(input, 4096, stdin);
        input[strcspn(input, "\n")] = '\0';
        SplitStrings(input, 4096, 0);
    }
}
