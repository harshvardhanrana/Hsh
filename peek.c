#include "headers.h"

extern char PrevDirectory[BUFFERLENGTH];
extern int Used;

extern char ShellStartLocation[BUFFERLENGTH];

void PrintStatistic(char* directory, char* file) {
    char file_location[BUFFERLENGTH];
    strcpy(file_location, directory);
    strcat(file_location, "/");
    strcat(file_location, file);
    struct stat fileinfo;
    
}

void PeekHandle(char *Location, int flaga, int flagl)
{

    char temp[256];

    if (Location[0] == '~')
    {
        strcpy(temp, ShellStartLocation);
        strcat(temp, &Location[1]);
    }
    else if (Location[0] == '-')
    {
        if (!Used)
        {
            printf("OLDPWD Not Set!\n");
            return;
        }
        else
        {
            strcpy(temp, PrevDirectory);
            strcat(temp, &Location[1]);
        }
    }
    else
    {
        strcpy(temp, Location);
    }

    DIR *directory;
    struct dirent *en;
    directory = opendir(temp);
    if (directory)
    {
        while ((en = readdir(directory)) != NULL)
        {
            if (flaga || (*en->d_name != '.'))
                if (flagl)
                    printf("%s\n", en->d_name);
                else
                    PrintStatistic(temp, en->d_name);
        }
        closedir(directory);
    }
    else {
        printf("Invalid Location: %s\n", Location);
    }
}

void ProcessPeek(char **Arguments)
{
    int flagindex = 1;
    int flaga = 0;
    int flagl = 0;
    while (Arguments[flagindex] != NULL
     && (Arguments[flagindex][0] == '-' && (Arguments[flagindex][1] == 'a' || Arguments[flagindex][1] == 'l')))
    {
        for (int i = 1; Arguments[1][i] != '\0'; i++)
        {
            if (Arguments[1][i] == 'a')
            {
                flaga = 1;
            }
            else if (Arguments[1][i] == 'l')
            {
                flagl = 1;
            }
            else
            {
                // Error Handle
            }
        }
        flagindex++;
    }
    if (Arguments[flagindex] == NULL)
    {
        PeekHandle(".", flaga, flagl);
    }
    else
    {
        PeekHandle(Arguments[flagindex], flaga, flagl);
    }
}