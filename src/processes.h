#ifndef PROCESSES_H
#define PROCESSES_H

#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

typedef struct {
    pid_t *list;
    int length;
} PidList;

/**
 * @brief The structure for a buffer.
 * @param main_pipe_fd The file descriptor for the main pipe.
 * @param reverse_pipe_fd The file descriptor for the reverse pipe.
 * @note Defined in processes.h
 */
typedef struct {
    int main_pipe_fd[2];
    int reverse_pipe_fd[2];
    PidList pid_list;
} Buffer;

/**
 * @brief The structure for an item.
 * @param line The line of the item.
 * @param column The column of the item.
 * @param type The type of the item.
 * @param dimension The dimension of the item.
 * @param speed The speed of the item.
 * @param direction The direction of the item.
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
#define MSLEEP_INTEVAL 100
#define MSEC_IN_SEC 1000
#define SLEEP_TIME 3

typedef enum { PIPE_READ, PIPE_WRITE } PipeAction;
typedef enum { MAIN_PIPE, REVERSE_PIPE } Pipe;

Buffer newBuffer();
void newTask(Buffer *buffer, void (*func)(Buffer, Item), Item *item);
void killTask(Item *item);
void writeItem (Buffer *buffer, Item *item, Pipe pipe);
void readItem (Buffer *buffer, Item *item, Pipe pipe);
void msleep(time_t t); // Sleeps for t milliseconds.

#endif // PROCESSES_H