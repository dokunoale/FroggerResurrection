#ifndef THREADS_H
#define THREADS_H

#define BUFFER_SIZE 10
#define PROD_RATE 1
#define CONS_RATE 2

#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

void writeItem(Buffer buffer, Item *item);
void readItem(Buffer buffer, Item *item);

#endif // THREADS_H