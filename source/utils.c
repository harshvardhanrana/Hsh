#include "../headers/headers.h"

void StringReplace(char* BigString, char* ToReplace, char* ReplaceWith) {
    int index = 0;
    char NewString[4096];
    while (BigString[index] != '\0') {
        int ind2 = 0;
        while (ToReplace[ind2] != '\0' && BigString[index + ind2] != '\0' && BigString[index + ind2] == ToReplace[ind2]) {
            ind2++;
        }
        if (ToReplace[ind2] == '\0') {
            BigString[index] = '\0';
            strcpy(NewString, BigString);
            strcat(NewString, ReplaceWith);
            strcat(NewString, &BigString[index+ind2]);
            strcpy(BigString, NewString);
            // printf("%s |||| %s |||| %s\n", BigString, ToReplace, ReplaceWith);
            return;
        }
        index++;
    }
}

int StringtoInt(char* NumString) {
    int num = 0;
    for (int i=0; NumString[i] <= '9' && NumString[i] >= '0'; i++) {
        num *= 10;
        num += (int)NumString[i] - '0';
    }
    return num;
}