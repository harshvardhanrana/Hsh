#include "../headers/headers.h"

extern char ShellStartLocation[BUFFERLENGTH];

char Pastevents[15][4096];
int Start = 0;
int NumElems = 0;
int PastEventError = 0;


void WriteIntoFile() {
    char pasthistorypath[BUFFERLENGTH];
    strcpy(pasthistorypath, ShellStartLocation);
    strcat(pasthistorypath, "/pasthistory.bin");
    FILE* fptr = fopen(pasthistorypath, "w");
    fwrite(&NumElems, sizeof(int), 1, fptr);
    for (int i=0; i<NumElems; i++) {
        fwrite(Pastevents[(Start + i)%15], 4096*sizeof(char), 1, fptr);
    }
    fclose(fptr);
}

void ReadFromFile() {
    char pasthistorypath[BUFFERLENGTH];
    strcpy(pasthistorypath, ShellStartLocation);
    strcat(pasthistorypath, "/pasthistory.bin");
    FILE* fptr = fopen(pasthistorypath, "r");
    if (fptr == NULL) {
        NumElems = 0;
        return;
    }
    fread(&NumElems, sizeof(int), 1, fptr);
    for (int i=0; i<NumElems; i++) {
        fread(Pastevents[i], 4096*sizeof(char), 1, fptr);
    }
    fclose(fptr);
}

void PrintPast()
{
    if (NumElems == 0)
        printf("No Event Found!\n");
    for (int i = 0; i < NumElems; i++)
    {
        printf("%s\n", Pastevents[(Start + i) % 15]);
    }
}

void PastExecute(char *IndexStr, char* BigString, char* ToReplace)
{
    int Index = 0;
    for (int i = 0; IndexStr[i] != '\0'; i++)
    {
        Index *= 10;
        Index += (int)(IndexStr[i] - '0');
    }
    if (Index > NumElems) {
        PastEventError = 1;
        return;
    }
    int RealIndex = NumElems - Index;
    char temp[4096];
    strcpy(temp ,Pastevents[(Start+RealIndex)%15]);
    SplitStrings(temp, 4096, 1);
    StringReplace(BigString, ToReplace, Pastevents[(Start+RealIndex)%15]);
}


void ProcessPast(char **Arguments, char* OriginalString, char* TokenString)
{
    if (Arguments[1] == NULL)
        PrintPast();
    else if (strcmp(Arguments[1], "purge") == 0) {
        NumElems = 0;
        WriteIntoFile();
    }
    else if (strcmp(Arguments[1], "execute") == 0) {
        if (Arguments[2] == NULL) {
            PrintError("No argument found after execute!\n");
            return;
        }
        PastExecute(Arguments[2], OriginalString, TokenString);
    }
    else {
        PrintError("Invalid Argument: %s\n", Arguments[1]);
    }
}

void AddHistory(char* InputString) {
    strcpy(Pastevents[(Start+NumElems)%15], InputString);
    if (NumElems < 15)
        NumElems++;
    else 
        Start = (Start+1)%15;
    WriteIntoFile();
}

void RemoveLastPast() {
    NumElems--;
}

int CheckPastEvent(const char* InputString) {
    if (NumElems == 0)
        return 0;
    else {
        return strcmp(Pastevents[(Start + NumElems - 1) % 15], InputString) == 0;
    }
}

int PastEventsStringReplace(char* InputString, int BufferSize) {
    const int pasteventsize = 10;
    const int executesize = 7;
    int flag = 1;
    int tosave = 0;
    int execute = 0;
    int StringStart = 0;
    int StringEnd = 0;
    for (int i=0; i+pasteventsize<BufferSize && InputString[i] != '\0'; i++) {
        if (InputString[i] == ' ' || InputString[i] == '\t' || InputString[i] == '\n')
            continue;
        if (flag) {
            StringStart = i;
            char temp = InputString[i+pasteventsize];
            InputString[i+pasteventsize] = '\0';
            flag = strcmp(&InputString[i], "pastevents");
            InputString[i+pasteventsize] = temp;
            if (!flag) {
                i += pasteventsize;
            }
        }
        else if (!execute) {
            tosave = 1;
            char temp = InputString[i+executesize];
            InputString[i+executesize] = '\0';
            flag = strcmp(&InputString[i], "execute");
            InputString[i+executesize] = temp;
            if (!flag) {
                execute = 1;
                i += executesize;
            }
        }
        else {
            tosave = 0;
            int j = i;
            int index = 0;
            while (InputString[j] != '\0' && InputString[j] != '|' && InputString[j] != ';' &&
             InputString[j] != '&' && InputString[j] != '<' && InputString[j] != '>'
              && InputString[j] != ' ' && InputString[j] != '\t') {
                index *= 10;
                if (InputString[j] >= '0' && InputString[j] <= '9') {
                    index += (int)InputString[j] - '0';
                }
                else {
                    PrintError("Invalid Character in Index!\n");
                    return -1;
                }
                j++;
            }
            StringEnd = j;
            if (index > NumElems || index <= 0) {
                PrintError("Invalid Index: %d\n", index);
                PastEventError = 1;
                return -1;
            }
            char Newstring[4096];
            InputString[StringStart] = '\0';
            strcpy(Newstring, InputString);
            strcat(Newstring, Pastevents[(Start+NumElems - index)%15]);
            i = strlen(Newstring)-1;
            execute = 0;
            flag = 1;
            strcat(Newstring, &InputString[StringEnd]);
            strcpy(InputString, Newstring);
        }
    }
    if (!flag)
        tosave = 1;
    return tosave;
}