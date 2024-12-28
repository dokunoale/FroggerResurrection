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

/**
 * @brief The structure for an item.
 * @note Defined in struct.h  
 */
typedef struct {
    unsigned int line;
    int column;
    int type;
    unsigned int dimension;
    unsigned int speed;
    pid_t id;
} Item;

#define PID_CHILD 0
#define PIPE_READ 0
#define PIPE_WRITE 1

Buffer newBuffer();
void newTask(Buffer *buffer, void (*func)(), Item *item);
void killTask(Buffer *buffer, Item *item);
void writeItem (Buffer *buffer, Item *item);
void readItem (Buffer *buffer, Item *item);

#endif // PROCESSES_H