#ifndef PROCESSES_H
#define PROCESSES_H

#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

#define PID_CHILD 0
#define PIPE_READ 0
#define PIPE_WRITE 1

#endif // PROCESSES_H