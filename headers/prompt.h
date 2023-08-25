#ifndef __PROMPT_H
#define __PROMPT_H

void SetShellStartLocation();
void MakeRelative(char *buffer, const int BufferLength);

void AddForegroundProc(const char* ForegroundProc, int ExecTime);

void prompt();

#endif