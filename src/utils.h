#ifndef UTILS_H
#define UTILS_H

#include <unistd.h>
#include <stdlib.h>

#include "processes.h"

/**
 * @brief The function that generates a random number between min and max.
 * @note Defined in utils.h
 */
int choose(int min, int max) { return rand() % (max - min + 1) + min; }

#endif // UTILS_H