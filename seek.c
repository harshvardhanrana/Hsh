#include "headers.h"

char FilesBuffer[50][BUFFERLENGTH];
int NumFound = 0;

int IsDirectory(const char* location) {
    struct stat st;
    stat(location, &st);
    return S_ISDIR(st.st_mode);
}

int FileNameComp(const char* name_search, const char* filename) {
    int index = 0;
    while (name_search[index] == filename[index]) {
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

void seekdfs(const char* name_search, const char* direc, int flagd, int flagf) {
    DIR *directory;
    struct dirent *en;
    struct dirent **files;
    int numfiles = scandir(direc, &files, NULL, alphasort);
    int index = 0;

    while (index < numfiles) {
        en = files[index];
        if (en->d_name[0] == '.' && ((en->d_name[1] == '.' && en->d_name[2] == '\0') || en->d_name[1] == '\0')) {
            index++;
            continue;
        }
        char filepath[BUFFERLENGTH];
        strcpy(filepath, direc);
        strcat(filepath, "/");
        strcat(filepath, en->d_name);
        if (IsDirectory(filepath)) {
            if (flagd && strcmp(name_search, en->d_name) == 0) {
                strcpy(FilesBuffer[NumFound], filepath);
                NumFound++;
            }
            seekdfs(name_search, filepath, flagd, flagf);
        }
        else if (flagf && FileNameComp(name_search, en->d_name)) {
            strcpy(FilesBuffer[NumFound], filepath);
            NumFound++;
        }
        index++;
    }
}

void seek(char **Arguments) {
    NumFound = 0;
    int flagindex = 1;
    int flagd = 0;
    int flagf = 0;
    int flage = 0;
    while (Arguments[flagindex] != NULL
     && (Arguments[flagindex][0] == '-' && (Arguments[flagindex][1] == 'f' || Arguments[flagindex][1] == 'd' || Arguments[flagindex][1] == 'e')))
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
            else {
                // Error Handle
            }
        }
        flagindex++;
    }
    if (Arguments[flagindex] == NULL) {
        // Error Handle
        return;
    }
    if (flagf == 1 && flagd == 1) {
        // Error Handle
        return;
    }
    if (flagf == 0 && flagd == 0) {
        flagf = 1;
        flagd = 1;
    }
    if (Arguments[flagindex + 1] == NULL) {
        seekdfs(Arguments[flagindex], ".", flagd, flagf);
    }
    else {
        seekdfs(Arguments[flagindex], Arguments[flagindex+1], flagd, flagf);
    }
    for (int i=0; i<NumFound; i++) {
        printf("%s\n", FilesBuffer[i]);
    }
}