#include "headers.h"

char PrevDirectory[128] = {'\0'};
int Used = 0;

extern char ShellStartLocation[BUFFERLENGTH];


int WarpSeperate(const char* Location) {
    int status = 1;
    char temp[BUFFERLENGTH];
    getcwd(temp, BUFFERLENGTH);

    if (strcmp(Location, "-") == 0) {
        if (Used)
            status = chdir(PrevDirectory);
    }
    else if (strcmp(Location, "~") == 0) {
        status = chdir(ShellStartLocation);
    }
    else {
        status = chdir(Location);
    }
    
    if (status != 0) {
        return 0;
    }
    strcpy(PrevDirectory, temp);
    Used = 1;
    return 1;
}


void warp(char** Arguments) {
    int status = 1;
    

    int index = 1;

    while (Arguments[index] != NULL && status == 1) {
        if ((Arguments[index][0] == '~' || Arguments[index][0] == '-') && Arguments[index][1] == '/') {
            Arguments[index][1] = '\0';
            status &= WarpSeperate(Arguments[index]);
            status &= WarpSeperate(&Arguments[index][2]);
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
        perror("warp failed");
        WarpSeperate(PrevDirectory);
    }
}
