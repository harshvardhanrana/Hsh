#ifndef HEADERS_H_
#define HEADERS_H_


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <assert.h>
#include <sys/types.h>
#include <time.h>
#include <stdarg.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <termios.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "prompt.h"
#include "command.h"
#include "backgroundproc.h"
#include "warp.h"
#include "peek.h"
#include "pastevents.h"
#include "utils.h"
#include "proclore.h"
#include "seek.h"
#include "errorhandle.h"
#include "redirection.h"
#include "piping.h"
#include "signals.h"
#include "main.h"
#include "neonate.h"
#include "iman.h"

#define BUFFERLENGTH 256

#endif