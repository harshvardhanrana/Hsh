#ifndef __SIGNALS_H
#define __SIGNALS_H

int Ping(char** Args);

int bg(char* arg);
int fg(char* arg);
void interruptfg(int dummy);
void sendtobg(int dummy);

#endif