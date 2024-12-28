#ifndef UTILS_H
#define UTILS_H

#include <unistd.h>
#include <stdlib.h>

#include "processes.h"


int rand_num (int min, int max)
{
    return rand() % (max-min) + min;
}

#endif // UTILS_H