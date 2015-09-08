#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "include/timer.h"

int msSleep(long ms) {

    struct timeval tv;

    tv.tv_sec = 0;
    tv.tv_usec = ms;

    return select(0, NULL, NULL, NULL, &tv);
}
