#include "headers.h"

extern char PrevDirectory[BUFFERLENGTH];
extern int Used;

extern char ShellStartLocation[BUFFERLENGTH];

void PrintPermission(struct stat fileinfo) {
    printf( (S_ISDIR(fileinfo.st_mode)) ? "d" : "-");
    printf( (fileinfo.st_mode & S_IRUSR) ? "r" : "-");
    printf( (fileinfo.st_mode & S_IWUSR) ? "w" : "-");
    printf( (fileinfo.st_mode & S_IXUSR) ? "x" : "-");
    printf( (fileinfo.st_mode & S_IRGRP) ? "r" : "-");
    printf( (fileinfo.st_mode & S_IWGRP) ? "w" : "-");
    printf( (fileinfo.st_mode & S_IXGRP) ? "x" : "-");
    printf( (fileinfo.st_mode & S_IROTH) ? "r" : "-");
    printf( (fileinfo.st_mode & S_IWOTH) ? "w" : "-");
    printf( (fileinfo.st_mode & S_IXOTH) ? "x " : "- ");

}

void PrintStatistic(char* directory, char* file) {
    char file_location[BUFFERLENGTH];
    strcpy(file_location, directory);
    strcat(file_location, "/");
    strcat(file_location, file);
    struct stat fileinfo;
    if (stat(file_location, &fileinfo) == -1) {
        printf("Unable to Call Stat for %s!\n", file_location);
        return;
    }
    PrintPermission(fileinfo);
    printf("%3ld ", fileinfo.st_nlink);
    printf("%15s ", getpwuid(fileinfo.st_uid)->pw_name);
    printf("%15s ", getgrgid(fileinfo.st_gid)->gr_name);
    printf("%13ld ", fileinfo.st_size);
    char date[64];
    if (time(0) - fileinfo.st_mtime >= 6*31*24*60*60)
        strftime(date, 64, "%b %d  %Y", localtime(&(fileinfo.st_mtime)));
    else
        strftime(date, 64, "%b %d %H:%M", localtime(&(fileinfo.st_mtime)));
    printf("%s ",date);
    printf("%s\n",file);
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
    struct dirent **files;
    int numfiles = scandir(temp, &files, NULL, alphasort);
    int index = 0;
    if (numfiles < 0) {
        printf("Unable to access scandir\n");
        return;
    }
    while (index < numfiles) {
        en = files[index];
        if (flaga || (*en->d_name != '.'))
            if (!flagl)
                printf("%s\n", en->d_name);
            else
                PrintStatistic(temp, en->d_name);
        index++;
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
                printf("Invalid Flag Found!\n");
                return;
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