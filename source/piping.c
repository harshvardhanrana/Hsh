#include "../headers/headers.h"

int CheckRedirection(char *Input1, int Flag, char *OriginalInput)
{
    int readflag = 0;
    int ogin = dup(0);
    int ogout = dup(1);
    int writeflag = 0;
    char Input[256];
    strcpy(Input, Input1);
    for (int index = 0; Input[index] != '\0'; index++)
    {
        if (Input[index] == '<' || Input[index] == '>') {
            int redirectindex = index;
            if (Input[index] == '<') {
                if (readflag) {
                    PrintError("Can only redirect input from file once!\n");
                    return 1;
                }
                readflag = 1; // reading from file
            }
            else if (Input[index+1] == '>') {
                if (writeflag) {
                    PrintError("Can only redirect output to file once!\n");
                    return 1;
                }
                writeflag = 1; // appending
                index++;
            }
            else {
                if (writeflag) {
                    PrintError("Can only redirect output to file once!\n");
                    return 1;
                }
                writeflag = 2; // writing
            }
            index++;
            while (Input[index] == ' ' || Input[index] == '\t') {
                index++;
            }
            int filestart = index;
            while (Input[index] != ' ' && Input[index] != '\t' && Input[index] != '<'
            && Input[index] != '>' && Input[index] != '\0') {
                index++;
            }
            int fd;
            Input[index] = '\0';
            if (readflag == 1) {
                if ((fd = open(&Input[filestart], O_RDONLY, S_IWUSR | S_IRUSR)) < 0) {
                    PrintError("Error redirecting file: %s", &Input[filestart]);
                    return 1;
                }
                if (dup2(fd, STDIN_FILENO) < 0)
                {
                    PrintError("Error redirecting input");
                    return 1;
                }
                readflag = 2;
            }
            else if (writeflag == 1) {
                if ((fd = open(&Input[filestart], O_WRONLY | O_APPEND | O_CREAT, S_IWUSR | S_IRUSR)) < 0) {
                    PrintError("Error redirecting file: %s", &Input[filestart]);
                    return 1;
                }
                if (dup2(fd, STDOUT_FILENO) < 0)
                {
                    PrintError("Error redirecting output");
                    return 1;
                }
                writeflag = 3;
            }
            else if (writeflag == 2) {
                if ((fd = open(&Input[filestart], O_WRONLY | O_TRUNC | O_CREAT, S_IWUSR | S_IRUSR)) < 0) {
                    PrintError("Error redirecting file: %s", &Input[filestart]);
                    return 1;
                }
                if (dup2(fd, STDOUT_FILENO) < 0)
                {
                    PrintError("Error redirecting output");
                    return 1;
                }
                writeflag = 3;
            }
            char temp[256];
            Input[redirectindex] = '\0';
            strcpy(temp, Input);
            int fileend = index;
            strcat(temp, &Input[index+1]);
            strcpy(Input, temp);
            index = redirectindex-1;
            close(fd);
        }
    }
    ProcessInput(Input, Flag, OriginalInput);
    dup2(ogout, 1);
    dup2(ogin, 0);
    close(ogout);
    close(ogin);
    return 0;
}

void CreatePipe(int in, int out, char *Input, char *OriginalInput, int Flag)
{

    if (in != 0)
    {
        dup2(in, 0);
        close(in);
    }
    if (out != 1)
    {
        dup2(out, 1);
        close(out);
    }
    CheckRedirection(Input, 0, OriginalInput);
}

void ProcessPipe(char *Input, int Flag, char *OriginalInput)
{
    int pipefound = 0;
    int StringStart = 0;
    pid_t pid;
    int in, fd[2];
    int ogout = dup(1);
    int ogin = dup(0);
    in = 0;

    for (int index = 0; Input[index] != '\0'; index++)
    {
        if (Input[index] == '|')
        {
            pipefound = 1;
            Input[index] = '\0';
            pipe(fd);
            CreatePipe(in, fd[1], &Input[StringStart], OriginalInput, Flag);

            close(fd[1]);

            in = fd[0];

            StringStart = index + 1;
        }
    }
    if (pipefound == 0)
    {
        CheckRedirection(Input, Flag, OriginalInput);
        return;
    }
    if (in != 0)
        dup2(in, 0);

    dup2(ogout, 1);

    CheckRedirection(&Input[StringStart], Flag, OriginalInput);
    dup2(ogin, 0);
    close(ogin);
    close(ogout);
}
