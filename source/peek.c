#include "../headers/headers.h"

extern char PrevDirectory[BUFFERLENGTH];
extern int Used;

extern char ShellStartLocation[BUFFERLENGTH];

void PrintPermission(struct stat fileinfo)
{
    printf((S_ISDIR(fileinfo.st_mode)) ? "d" : S_ISLNK(fileinfo.st_mode) ? "l" : "-");
    printf((fileinfo.st_mode & S_IRUSR) ? "r" : "-");
    printf((fileinfo.st_mode & S_IWUSR) ? "w" : "-");
    printf((fileinfo.st_mode & S_IXUSR) ? "x" : "-");
    printf((fileinfo.st_mode & S_IRGRP) ? "r" : "-");
    printf((fileinfo.st_mode & S_IWGRP) ? "w" : "-");
    printf((fileinfo.st_mode & S_IXGRP) ? "x" : "-");
    printf((fileinfo.st_mode & S_IROTH) ? "r" : "-");
    printf((fileinfo.st_mode & S_IWOTH) ? "w" : "-");
    printf((fileinfo.st_mode & S_IXOTH) ? "x " : "- ");
}

void PrintStatistic(char *directory, char *file)
{
    char file_location[BUFFERLENGTH];
    strcpy(file_location, directory);
    strcat(file_location, "/");
    strcat(file_location, file);
    struct stat fileinfo;
    if (lstat(file_location, &fileinfo) == -1)
    {
        printf("Unable to Call Stat for %s!\n", file_location);
        return;
    }
    PrintPermission(fileinfo);
    printf("%3ld ", fileinfo.st_nlink);
    printf("%15s ", getpwuid(fileinfo.st_uid)->pw_name);
    printf("%15s ", getgrgid(fileinfo.st_gid)->gr_name);
    printf("%13ld ", fileinfo.st_size);
    char date[64];
    if (time(0) - fileinfo.st_mtime >= 6 * 31 * 24 * 60 * 60)
        strftime(date, 64, "%b %d  %Y", localtime(&(fileinfo.st_mtime)));
    else
        strftime(date, 64, "%b %d %H:%M", localtime(&(fileinfo.st_mtime)));
    printf("%s ", date);
    printf("%s\n", file);
}

int GetBlockSize(char *directory, char *file)
{
    char file_location[BUFFERLENGTH];
    strcpy(file_location, directory);
    strcat(file_location, "/");
    strcat(file_location, file);
    struct stat fileinfo;
    if (lstat(file_location, &fileinfo) == -1)
    {
        PrintError("Unable to access stat for %s\n", file_location);
        return -1;
    }
    return fileinfo.st_blocks;
}

void ChangeColor(char *directory, char *file)
{
    char file_location[BUFFERLENGTH];
    strcpy(file_location, directory);
    strcat(file_location, "/");
    strcat(file_location, file);
    struct stat fileinfo;
    if (lstat(file_location, &fileinfo) == -1)
    {
        PrintError("Unable to access %s", file_location);
        return;
    }
    if (S_ISDIR(fileinfo.st_mode))
        printf("\033[1;34m");
    else if (S_IXUSR & fileinfo.st_mode)
        printf("\033[1;32m");
    else
        printf("\033[1;37m");
}

int PeekHandle(char *Location, int flaga, int flagl)
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
            PrintError("OLDPWD Not Set!\n");
            return -1;
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
    struct dirent **files;
    int numfiles = scandir(temp, &files, NULL, alphasort);
    int index = 0;
    if (numfiles < 0)
    {
        PrintError("Unable to access scandir for %s\n",temp);
        return -1;
    }
    if (flagl)
    {
        int sum = 0;
        while (index < numfiles)
        {
            en = files[index];
            if (flaga || (*en->d_name != '.'))
            {
                int blk = GetBlockSize(temp, en->d_name);
                if (blk < 0)
                {
                    return -1;
                }
                sum += blk;
            }
            index++;
        }
        printf("total %d\n", sum);
    }
    index = 0;
    while (index < numfiles)
    {
        en = files[index];
        if (flaga || (*en->d_name != '.'))
        {
            ChangeColor(temp, en->d_name);
            if (!flagl)
                printf("%s\n", en->d_name);
            else
                PrintStatistic(temp, en->d_name);
            printf("\033[0m");
        }
        free(files[index]);
        index++;
    }
    free(files);
    return 0;
}

int ProcessPeek(char **Arguments)
{
    int flagindex = 1;
    int flaga = 0;
    int flagl = 0;
    while (Arguments[flagindex] != NULL && (Arguments[flagindex][0] == '-' && (Arguments[flagindex][1] != '/' && Arguments[flagindex][1] != '\0')))
    {
        for (int i = 1; Arguments[1][i] != '\0'; i++)
        {
            if (Arguments[flagindex][i] == 'a')
            {
                flaga = 1;
            }
            else if (Arguments[flagindex][i] == 'l')
            {
                flagl = 1;
            }
            else
            {
                PrintError("Invalid Flag: %c\n", Arguments[flagindex][i]);
                return -1;
            }
        }
        flagindex++;
    }
    if (Arguments[flagindex] == NULL)
    {
        return PeekHandle(".", flaga, flagl);
    }
    else
    {
        return PeekHandle(Arguments[flagindex], flaga, flagl);
    }
    return 0;
}