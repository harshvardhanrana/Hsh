#include "../headers/headers.h"

int ExitFlag;

int IsEmptyString(char* Statement) {
    int index = 0;
    while (Statement[index] != '\0') {
        if (Statement[index] != ' ' && Statement[index] != '\t' && Statement[index] != ';' && Statement[index] != '&')
            return 0;
        index++;
    }
    return 1;
}

void ExecuteForegroundCommand(char **Arguments)
{
    int ex = fork();
    if (ex == 0)
    {
        int res = execvp(Arguments[0], Arguments);
        if (res == -1)
        {
            PrintError("'%s' is not a valid command\n", Arguments[0]);
        }
    }
    else
    {
        int y;
        waitpid(ex, &y, 0);
    }
}

extern int BackgroundProcCount;

void ExecuteBackgroundCommand(char **Arguments) {
    int ex = fork();
    if (ex == 0)
    {
        int res = execvp(Arguments[0], Arguments);
        if (res == -1)
        {
            printf("ERROR: '%s' is not a valid command\n", Arguments[0]);
        }
    }
    else {
        AddBackgroundProcess(Arguments[0], ex);
        printf("[%d] %d\n", BackgroundProcCount, ex);
    }
}

int IsPastEvent = 0;
extern int PastEventError;

void ProcessInput(char *Input, int Flag, char* OriginalInput)
{
    char Delimiters[5] = "\n\t ";

    char Command[256];
    char *argv[256];
    int index = 0;

    char InputTemp[256];
    strcpy(InputTemp, Input);

    char *token = strtok(Input, Delimiters);

    while (token != NULL)
    {
        argv[index] = malloc(sizeof(char) * 128);
        strcpy(argv[index++], token);
        token = strtok(NULL, Delimiters);
    }
    argv[index] = NULL;

    if (argv[0] == NULL) {
        return;
    }

    time_t start,end;
    start = time(NULL);

    if (strcmp(argv[0],"warp") == 0)
        warp(argv);
    else if (strcmp(argv[0],"peek") == 0)
        ProcessPeek(argv);
    else if (strcmp(argv[0],"proclore") == 0)
        Proclore(argv[1]);
    else if (strcmp(argv[0], "seek") == 0)
        seek(argv);
    else if (strcmp(argv[0], "exit") == 0)
        ExitFlag = 1;
    else if (strcmp(argv[0], "pastevents") == 0) {
        if (argv[1] == NULL || strcmp(argv[1],"execute")) {
            IsPastEvent = 1;
        }
        ProcessPast(argv, OriginalInput, InputTemp);
    }
    else if (Flag == 0) 
        ExecuteForegroundCommand(argv);
    else
        ExecuteBackgroundCommand(argv);

    end = time(NULL);
    if (!Flag && (end - start) >= 2) {
        AddForegroundProc(argv[0], (end - start));
    }
    for (int i=0; i<index; i++) {
        free(argv[i]);
    }
}

void SplitStrings(char *InputString, const int InputLength, int IsCalled)
{
    char temp[4096];
    strcpy(temp, InputString);
    int StringStart = 0;
    IsPastEvent = 0;
    PastEventError = 0;

    for (int index = 0; InputString[index] != '\0'; index++)
    {
        if (InputString[index] == '&' || InputString[index] == ';')
        {
            int flag = (InputString[index] == '&');
            InputString[index] = '\0';
            ProcessInput(&InputString[StringStart], flag, temp);
            StringStart = index + 1;
        }
    }
    ProcessInput(&InputString[StringStart], 0, temp);
    if (!IsEmptyString(temp) && !IsPastEvent && !CheckPastEvent(temp) && !IsCalled && !PastEventError)
        AddHistory(temp);

}
