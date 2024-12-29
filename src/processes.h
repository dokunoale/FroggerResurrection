#ifndef PROCESSES_H
#define PROCESSES_H

#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

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
 * @param line The line of the item.
 * @param column The column of the item.
 * @param type The type of the item.
 * @param dimension The dimension of the item.
 * @param speed The speed of the item.
 * @param id The PID of the item.
 * @note Defined in processes.h  
 */
typedef struct {
    unsigned int line;
    int column;
    int type;
    unsigned int dimension;
    unsigned int speed;
    unsigned int direction;
    pid_t id;
} Item;

#define PID_CHILD 0
#define PIPE_READ 0
#define PIPE_WRITE 1

Buffer newBuffer();
void newTask(Buffer *buffer, void (*func)(Buffer, Item), Item *item);
void killTask(Buffer *buffer, Item *item);
void writeItem (Buffer *buffer, Item *item);
void readItem (Buffer *buffer, Item *item);

#endif // PROCESSES_H