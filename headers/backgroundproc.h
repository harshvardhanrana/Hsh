#ifndef __BACKGROUNDPROC_H
#define __BACKGROUNDPROC_H

void AddBackgroundProcess(char* CommandName, int Pid);
void RemoveProcWithPid(int pid, char* CommandName);
void KillAllProcs();

void RemoveBackgroundBuffers();
int Activities();

#endif