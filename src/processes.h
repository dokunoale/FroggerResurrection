#ifndef PROCESSES_H
#define PROCESSES_H

#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>

#define _BSD_SOURCE
#include <unistd.h>

#define MSLEEP_INTEVAL 100
#define MSEC_IN_SEC 1000

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

/**
 * @param line The line of the flow.
 * @param direction The direction of the flow: LEFT or RIGHT.
 * @param speed The speed of the flow.
 * @param how_many_crocodiles The number of crocodiles in the flow.
 * @param crocodiles An array of crocodiles of type Item.
 */
typedef struct Flow {
    unsigned int line;
    unsigned int direction;
    unsigned int speed;
    Item *crocodiles;
    Item *bullets;
    Item *granades;
} Flow;

#define PID_CHILD 0

typedef enum { PIPE_READ, PIPE_WRITE } PipeAction;
typedef enum { MAIN_PIPE, REVERSE_PIPE } Pipe;

Buffer newBuffer();
void newTask(Buffer *buffer, void (*func)(Buffer, Item), Item *item);
void killTask(Item *item);
void writeItem (Buffer *buffer, Item *item, Pipe pipe);
ssize_t readItem (Buffer *buffer, Item *item, Pipe pipe);

#endif // PROCESSES_H