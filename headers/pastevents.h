#ifndef __PASTEVENT_H
#define __PASTEVENT_H

void ProcessPast();
void AddHistory();
void RemoveLastPast();
int CheckPastEvent(const char*);
void ReadFromFile();
int PastEventsStringReplace(char* InputString, int BufferSize);

#endif