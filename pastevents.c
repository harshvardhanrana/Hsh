#include "headers.h"

char Pastevents[15][4096];
int Start = 0;
int NumElems = 0;
int PastEventError = 0;


void WriteIntoFile() {
    FILE* fptr = fopen("pasthistory.bin", "w");
    fwrite(&NumElems, sizeof(int), 1, fptr);
    for (int i=0; i<NumElems; i++) {
        fwrite(Pastevents[(Start + i)%15], 4096*sizeof(char), 1, fptr);
    }
}

void ReadFromFile() {
    FILE* fptr = fopen("pasthistory.bin", "r");
    fread(&NumElems, sizeof(int), 1, fptr);
    for (int i=0; i<NumElems; i++) {
        fread(Pastevents[i], 4096*sizeof(char), 1, fptr);
    }
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
        printf("Invalid Index\n");
        PastEventError = 1;
        return;
    }
    int RealIndex = NumElems - Index;
    char temp[4096];
    strcpy(temp ,Pastevents[RealIndex%15]);
    SplitStrings(temp, 4096, 1);
    StringReplace(BigString, ToReplace, Pastevents[RealIndex%15]);
}


void ProcessPast(char **Arguments, char* OriginalString, char* TokenString)
{
    if (Arguments[1] == NULL)
        PrintPast();
    else if (strcmp(Arguments[1], "purge") == 0) {
        NumElems = 0;
        WriteIntoFile();
    }
    else if (strcmp(Arguments[1], "execute") == 0)
        PastExecute(Arguments[2], OriginalString, TokenString);
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