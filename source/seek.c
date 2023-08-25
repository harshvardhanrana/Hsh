#include "../headers/headers.h"

char FirstFile[1024] = {'\0'};
char *LocationToSearch;

int NumFound = 0;
int FileFound = 0, FileDir = -1;
int flagd, flagf, flage = 0;

extern char ShellStartLocation[BUFFERLENGTH];

void PrintRelative(const char *location, int Dir)
{
    char Relative[BUFFERLENGTH];
    strcpy(Relative, "./");
    if (LocationToSearch != NULL)
        strcat(Relative, &location[strlen(LocationToSearch) + 1]);
    else
        strcat(Relative, &location[2]);
    if (Dir)
        printf("\033[1;34m");
    else
        printf("\033[1;32m");
    printf("%s\n\033[1;0m", Relative);
}

void HandleFileOrDir(char *filepath, int Dir)
{
    if (!flage || FileFound == -1)
    {
        PrintRelative(filepath, Dir);
    }
    else if (FileFound == 1)
    {
        FileFound = -1;
        PrintRelative(FirstFile, FileDir);
        PrintRelative(filepath, Dir);
    }
    else
    {
        FileFound = 1;
        strcpy(FirstFile, filepath);
        FileDir = Dir;
    }
    NumFound++;
}

int IsDirectory(const char *location)
{
    struct stat st;
    int status = lstat(location, &st);
    return S_ISDIR(st.st_mode) && (status != -1);
}

int FileNameComp(const char *name_search, const char *filename)
{
    int index = 0;
    while (name_search[index] == filename[index])
    {
        if (name_search[index] == '\0')
            break;
        index++;
    }
    if (name_search[index] != '\0')
        return 0;
    if (filename[index] == '.' || filename[index] == '\0')
        return 1;
    return 0;
}

void seekdfs(const char *name_search, const char *direc)
{
    struct dirent *en;
    struct dirent **files;
    int numfiles = scandir(direc, &files, NULL, alphasort);
    if (numfiles < 0)
    {
        // PrintError("Unable to execute scandir on directory: %s\n", direc);
        return;
    }
    int index = 0;

    while (index < numfiles)
    {
        en = files[index];
        if (en->d_name[0] == '.' && ((en->d_name[1] == '.' && en->d_name[2] == '\0') || en->d_name[1] == '\0'))
        {
            free(files[index]);
            index++;
            continue;
        }
        char filepath[BUFFERLENGTH];
        strcpy(filepath, direc);
        strcat(filepath, "/");
        strcat(filepath, en->d_name);
        if (IsDirectory(filepath))
        {
            if (flagd && strcmp(name_search, en->d_name) == 0)
            {
                HandleFileOrDir(filepath, 1);
            }
            seekdfs(name_search, filepath);
        }
        else if (flagf && FileNameComp(name_search, en->d_name))
        {
            HandleFileOrDir(filepath, 0);
        }
        free(files[index]);
        index++;
    }
    free(files);
}

void seek(char **Arguments)
{
    LocationToSearch = 0;
    FileFound = 0, FileDir = -1;
    NumFound = 0;
    int flagindex = 1;
    flagd = 0;
    flagf = 0;
    flage = 0;
    while (Arguments[flagindex] != NULL && (Arguments[flagindex][0] == '-' && (Arguments[flagindex][1] == 'f' || Arguments[flagindex][1] == 'd' || Arguments[flagindex][1] == 'e')))
    {
        for (int i = 1; Arguments[1][i] != '\0'; i++)
        {
            if (Arguments[flagindex][i] == 'd')
            {
                flagd = 1;
            }
            else if (Arguments[flagindex][i] == 'f')
            {
                flagf = 1;
            }
            else if (Arguments[flagindex][i] == 'e')
            {
                flage = 1;
            }
            else
            {
                PrintError("Invalid Flag Found: %c\n", Arguments[flagindex][i]);
                return;
            }
        }
        flagindex++;
    }
    if (Arguments[flagindex] == NULL)
    {
        PrintError("No Argument Found for Seek!\n");
        return;
    }
    if (flagf == 1 && flagd == 1)
    {
        PrintError("Invalid flags!\n");
        return;
    }
    if (flagf == 0 && flagd == 0)
    {
        flagf = 1;
        flagd = 1;
    }
    LocationToSearch = Arguments[flagindex + 1];
    if (Arguments[flagindex + 1] == NULL)
    {
        seekdfs(Arguments[flagindex], ".");
    }
    else
    {
        if (Arguments[flagindex + 1][0] == '~')
        {
            char temp[BUFFERLENGTH];
            strcpy(temp, ShellStartLocation);
            strcat(temp, &Arguments[flagindex + 1][1]);
            strcpy(Arguments[flagindex + 1], temp);
        }
        if (!IsDirectory(Arguments[flagindex + 1]))
        {
            PrintError("%s is not a Directory!\n");
            return;
        }
        seekdfs(Arguments[flagindex], Arguments[flagindex + 1]);
    }
    if (NumFound == 0)
    {
        printf("No match found!\n");
        return;
    }
    if (NumFound < 0)
    {
        return;
    }
    if (FileFound == 1)
    {
        if (IsDirectory(FirstFile))
        {
            if (WarpSeperate(FirstFile) == 0) {
                printf("Missing permissions for task!\n");
            }
            else {
                printf("Warped to ");
                PrintRelative(FirstFile, 1);
            }
        }
        else
        {
            PrintRelative(FirstFile, 0);
            FILE *fptr = fopen(FirstFile, "r");
            if (fptr == NULL)
            {
                printf("Missing permissions for task!\n");
                return;
            }
            char str[50];
            while (fgets(str, 50, fptr) != NULL)
            {
                printf("%s", str);
            }
        }
    }
}