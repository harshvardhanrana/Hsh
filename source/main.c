#include "../headers/headers.h"

extern int ExitFlag;

void die(const char *s) {
    perror(s);
    exit(1);
}

struct termios orig_termios;

void disableRawMode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

void enableRawMode() {
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}

int main()
{
    signal(SIGINT, interruptfg);
    SetShellStartLocation();
    ReadFromFile();
    // Keep accepting commands
    while (1)
    {
        ExitFlag = 0;
        // Print appropriate prompt with username, systemname and directory before accepting input
        prompt();
        char input[4096];
        fgets(input, 4096, stdin);
        input[strcspn(input, "\n")] = '\0';
        RemoveBackgroundBuffers();
        SplitStrings(input, 4096, 0);
        if (ExitFlag)
            exit(0);
    }
}
