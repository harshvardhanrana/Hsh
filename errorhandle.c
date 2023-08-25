#include "headers.h"

// void PrintError(const char* ErrorMessage) {
//     if (ErrorMessage == NULL) {
//         printf("\033[1;31mError!\n\033[1;0m");
//     }
//     else {
//         printf("\033[1;31m%s\n\033[1;0m",ErrorMessage);
//     }
// }

void PrintError(const char *ErrorMessage, ...) {
    char buffer[4096];
    va_list args;
    va_start(args, ErrorMessage);
    int rc = vsnprintf(buffer, sizeof(buffer), ErrorMessage, args);
    va_end(args);
    printf("\033[1;31m[Error] %s\033[1;0m",buffer);
}
