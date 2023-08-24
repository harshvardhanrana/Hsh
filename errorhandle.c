#include "headers.h"

void PrintError(const char* ErrorMessage) {
    if (ErrorMessage == NULL) {
        printf("\033[1;31mError!\n\033[1;0m");
    }
    else {
        printf("\033[1;31m%s\n\033[1;0m",ErrorMessage);
    }
}
