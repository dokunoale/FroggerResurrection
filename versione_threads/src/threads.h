#ifndef THREADS_H
#define THREADS_H

#define BUFFER_SIZE 16
#define PROD_RATE 1
#define CONS_RATE 2
#define THREADS_NUM 50

#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <semaphore.h>

/**
 * @brief The structure for an item.
 * @param line The line of the item.
 * @param column The column of the item.
 * @param type The type of the item.
 * @param dimension The dimension of the item.
 * @param speed The speed of the item.
 * @param direction The direction of the item.
 * @param id The TID of the item.
 * @note Defined in processes.h  
 */
typedef struct {
    unsigned int line;
    int column;
    int type;
    unsigned int dimension;
    unsigned int speed;
    unsigned int direction;
    int stage;
    pthread_t id;
} Item;

typedef struct {
    // Item buffer[BUFFER_SIZE];
    int head; // Indice di scrittura
    int tail; // Indice di lettura
    int count; // Numero di elementi nel buffer
    pthread_mutex_t mutex;
    pthread_cond_t not_full;
    pthread_cond_t not_empty;
} buffer_t;

/**
 * @brief The structure for a buffer.
 * @param main_pipe_fd The file descriptor for the main pipe.
 * @param reverse_pipe_fd The file descriptor for the reverse pipe.
 * @note Defined in processes.h
 */
typedef struct {
    buffer_t* main_buf;
    buffer_t* reverse_buf; 
} Buffer;

typedef struct {
    Buffer buffer;
    Item item;
    void (*func)(Buffer, Item);
} args_t;

typedef enum { MAIN_BUF = 0, REVERSE_BUF } Buff;

Buffer newBuffer();
void freeBuffer(Buffer *buffer);
void newTask(Buffer *buffer, void (*func)(Buffer, Item), Item *item);
void killTask(Item *item);
void writeItem (Buffer *buffer, Item *item, Buff);
ssize_t readItem (Buffer *buffer, Item *item, Buff);

#endif // THREADS_H