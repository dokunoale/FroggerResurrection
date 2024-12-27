#ifndef PROCESSES_H
#define PROCESSES_H

#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

typedef struct {
    int *list;
    int length;
} PidList;

typedef struct {
    int pipe_fd[2];
    PidList *pid_list;
} Buffer;

#define PID_CHILD 0
#define PIPE_READ 0
#define PIPE_WRITE 1

Buffer newBuffer();
void newTask(int *pipe_fd, PidList *pid_list, int index, void (*func)(), int *args);
void writeItem (int pipe_fd, Item *item);
void readItem (int pipe_fd, Item *item);

#endif // PROCESSES_H