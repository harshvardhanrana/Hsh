#include "../headers/headers.h"

char PrevDirectory[BUFFERLENGTH] = {'\0'};
int Used = 0;

extern char ShellStartLocation[BUFFERLENGTH];


int WarpSeperate(const char* Location) {
    int status = 1;
    char temp[BUFFERLENGTH];
    getcwd(temp, BUFFERLENGTH);

    if (strcmp(Location, "-") == 0) {
        if (Used)
            status = chdir(PrevDirectory);
        else {
            PrintError("OLDPWD Not Set!\n");
        }
    }
    else if (strcmp(Location, "~") == 0) {
        status = chdir(ShellStartLocation);
        Used = 1;
    }
    else {
        status = chdir(Location);
        Used = 1;
    }
    
    if (status != 0) {
        status = chdir(temp);
        return 0;
    }
    strcpy(PrevDirectory, temp);
    Used = 1;
    return 1;
}


int warp(char** Arguments) {
    int status = 1;
    

    int index = 1;

    while (Arguments[index] != NULL && status == 1) {
        if ((Arguments[index][0] == '~' || Arguments[index][0] == '-') && Arguments[index][1] == '/') {
            char temp[BUFFERLENGTH];
            if (Arguments[index][0] == '~') {
                strcpy(temp, ShellStartLocation);
            }
            else {
                if (!Used) {
                    PrintError("OLDPWD Not Set!\n");
                    status = 0;
                }
                strcpy(temp, PrevDirectory);
            }
            strcat(temp, "/");
            strcat(temp, &Arguments[index][2]);
            status = status && WarpSeperate(temp);
        }
        else {
            status &= WarpSeperate(Arguments[index]);
        }
        index++;
    }
    
    if (Arguments[1] == NULL) {
        status &= WarpSeperate("~");
    }

    if (status == 0) {
        printf("Warp Failed for %s\n", Arguments[index-1]);
    }
    return (status == 0);
}
