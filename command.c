#include "headers.h"


void ProcessInput(char *Input, const int InputLength)
{
    char Delimiters[5] = "\n\t ";

    char Command[256];
    char *argv[256];
    int index = 0;

    char *token = strtok(Input, Delimiters);

    while (token != NULL)
    {
        argv[index] = malloc(sizeof(char) * 128);
        strcpy(argv[index++], token);
        token = strtok(NULL, Delimiters);
    }
    argv[index] = NULL;
    int ex = fork();
    if (ex == 0)
    {
        int res = execvp(argv[0], argv);
        if (res == -1)
        {
            printf("ded\n");
        }
    }
    else
    {
        int y;
        waitpid(ex, &y, 0);
    }
    
}
