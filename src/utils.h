#ifndef UTILS_H
#define UTILS_H

#include <unistd.h>
#include <stdlib.h>

#include "processes.h"


int choose(int min, int max) { return rand() % (max - min + 1) + min; }

#endif // UTILS_H