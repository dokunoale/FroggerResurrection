/**
 * @note TODO: Implement the thread version of the functions.
 */

#include "threads.h" 
#include "struct.h"

typedef struct {
    Item items[BUFFER_SIZE];
    int in;
    int out;
    int produced_count;
    int consumed_count;
    pthread_mutex_t mutexBuffer;
    sem_t semFull;
    sem_t semEmpty;
} buffer_t;

struct List_thread {
    pthread_t* list;
    int len;
};

typedef buffer_t* Buffer;

// Close the buffer
void close_t(Buffer buffer) { 
    sem_destroy(&buffer->semFull);
    sem_destroy(&buffer->semEmpty);
    pthread_mutex_destroy(&buffer->mutexBuffer);
    free(buffer);
}

void rest_t(int rate) {
    int usecs = 10000 * (rand() % (20 * rate) + (5 * rate) / 2);
    usleep(usecs);
}

void wait_t(sem_t* sem, pthread_mutex_t* mutex) { 
    sem_wait(sem); 
    pthread_mutex_lock(mutex);
}

void signal_t(sem_t* sem, pthread_mutex_t* mutex) { 
    sem_post(sem); 
    pthread_mutex_unlock(mutex);
}

void newTask(Buffer buffer, List_thread* tids, int index, void (*func)(), int *args) {
    if(pthread_create(&(tids->list[index]), NULL, func_thread, (void*) func_params) != 0) {
        tids->list[index] = 0;
        quit_manche(*tids);
        quit(ERR_INIT_THREAD);
    }
}

/**
 * @brief PRODUCER: Write an item in the buffer
 * @param buffer the buffer where to write the item
 * @param item the item to write in the buffer
 * @return the exit status of the function
 * @note Thread version
 */
void writeItem(Buffer buffer, Item *item) {
    sem_wait(&buffer->semEmpty);
    pthread_mutex_lock(&buffer->mutexBuffer);
    buffer->produced_count++;
    
    // add to the buffer
    buffer->items[buffer->in] = *item;
    buffer->in = (buffer->in + 1) % BUFFER_SIZE;

    pthread_mutex_unlock(&buffer->mutexBuffer);
    sem_post(&buffer->semFull);
    rest_t(PROD_RATE);
}

/**
 * @brief CONSUMER: Read an item from the buffer
 * @param buffer the buffer from where to read the item
 * @param item the item to read from the buffer
 * @return the exit status of the function
 * @note Thread version
 */
void readItem(Buffer buffer, Item* item) {
    sem_wait(&buffer->semFull);
    pthread_mutex_lock(&buffer->mutexBuffer);
    buffer->consumed_count++;

    // remove from the buffer
    *item = buffer->items[buffer->out];
    buffer->out = (buffer->out + 1) % BUFFER_SIZE;

    pthread_mutex_unlock(&buffer->mutexBuffer);
    sem_post(&buffer->semEmpty);
    rest_t(CONS_RATE);
}