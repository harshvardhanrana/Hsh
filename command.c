#include "headers.h"

void ExecuteCommand(char **Arguments)
{
    int ex = fork();
    if (ex == 0)
    {
        int res = execvp(Arguments[0], Arguments);
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

void ProcessInput(char *Input, int Flag)
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
    ExecuteCommand(argv);
}

void SplitStrings(char *InputString, const int InputLength)
{

    int StringStart = 0;

    for (int index = 0; InputString[index] != '\0'; index++)
    {
        if (InputString[index] == '&' || InputString[index] == ';')
        {
            int flag = (InputString[index] == '&');
            InputString[index] = '\0';
            ProcessInput(&InputString[StringStart], flag);
            StringStart = index + 1;
        }
    }
    ProcessInput(&InputString[StringStart], 0);
}
