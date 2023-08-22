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
#include <signal.h>

#include "prompt.h"
#include "command.h"
#include "backgroundproc.h"
#include "warp.h"
#include "peek.h"
#include "pastevents.h"
#include "utils.h"
#include "proclore.h"
#include "seek.h"

#define BUFFERLENGTH 256

#endif