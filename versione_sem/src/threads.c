#include "threads.h" 
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

pthread_t tid_list[THREADS_NUM] = {0};

Item main_buffer[BUFFER_SIZE];
Item reverse_buffer[BUFFER_SIZE];
buffer_t main_buf; // = {0, 0, 0, PTHREAD_MUTEX_INITIALIZER, 0, 0};
buffer_t reverse_buf; // = {0, 0, 0, PTHREAD_MUTEX_INITIALIZER, 0, 0};
args_t temp_args = {0};
pthread_mutex_t create_mutex = PTHREAD_MUTEX_INITIALIZER;

void store_tid(pthread_t tid) {
    for (int i = 0; i < THREADS_NUM; i++) {
        if (tid_list[i] == 0) {
            tid_list[i] = tid;
            return;
        }
    }
}

void delete_tid(pthread_t tid) {
    for (int i = 0; i < THREADS_NUM; i++) {
        if (tid_list[i] == tid) {
            tid_list[i] = 0;
            return;
        }
    }
}

int is_tid_stored(pthread_t tid) {
    for (int i = 0; i < THREADS_NUM; i++) {
        if (tid_list[i] == tid) {
            return 1;
        }
    }
    return 0;
}

void random_rest(int min, int max) {
    int usecs = 6000 * (rand() % (max - min + 1) + min);
    usleep(usecs);
}

// Initialize the mutex and the semaphores -- deprecated
void init_semaphores(buffer_t* cb) {
    cb->head = 0;
    cb->tail = 0;
    cb->count = 0;
    cb->mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    cb->not_full = sem_init(&cb->not_full, 0, BUFFER_SIZE);
    cb->not_empty = sem_init(&cb->not_empty, 0, 0);
}

// Distruggi il buffer -- deprecated
void free_semaphores(buffer_t* cb) {
    pthread_mutex_destroy(&cb->mutex);
    sem_destroy(&cb->not_full);
    sem_destroy(&cb->not_empty);
}

// Crea un buffer
Buffer newBuffer() {
    Buffer buffer;

    buffer.main_buf = &main_buf;
    buffer.reverse_buf = &reverse_buf;

    init_semaphores(buffer.main_buf);
    init_semaphores(buffer.reverse_buf);

    return buffer;
}

// Distruggi il buffer
void freeBuffer(Buffer* buffer) {
    free_semaphores(buffer->main_buf);
    free_semaphores(buffer->reverse_buf);

    Item cleaner = {0};
    for (int i = 0; i < BUFFER_SIZE; i++) {
        main_buffer[i] = cleaner;
        reverse_buffer[i] = cleaner;
    }
}

// Funzione per chiamare la funzione del thread
void* thread_func(void* arg) {
    args_t* args = (args_t*)arg;
    Buffer tmp_buffer = args->buffer;
    Item item = args->item;
    void (*func)(Buffer, Item) = args->func;
    pthread_mutex_unlock(&create_mutex);
    func(tmp_buffer, item);
    return NULL;
}

// Funzione per creare un nuovo thread
void newTask(Buffer *buffer, void (*func)(Buffer, Item), Item *item) {
    pthread_mutex_lock(&create_mutex);
    temp_args.buffer = *buffer;
    temp_args.item = *item;
    temp_args.func = func;
    if(pthread_create(&(temp_args.item.id), NULL, thread_func, &temp_args) != 0) {
        perror("Errore nella creazione del thread");
    }
    item->id = temp_args.item.id;
    store_tid(item->id);
}

// Scrittura nel buffer
void write_item(buffer_t* cb, Item* buf, Item* item) {

    // pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
    pthread_mutex_lock(&cb->mutex);
    
    // Attendi se il buffer è pieno
    while (cb->count >= BUFFER_SIZE) {
        pthread_mutex_unlock(&cb->mutex);
        random_rest(1, 2);
        pthread_mutex_lock(&cb->mutex);
    }
    pthread_testcancel();
    sem_wait(&cb->not_full);

    // Scrivi il valore nel buffer
    buf[cb->head] = *item;
    cb->head = (cb->head + 1) % BUFFER_SIZE;
    cb->count++;

    // Segnala che il buffer non è vuoto
    sem_post(&cb->not_empty);
    pthread_mutex_unlock(&cb->mutex);
    // pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

    // random_rest(5, 10);
}   

// Lettura dal buffer
void read_item(buffer_t* cb, Item* buf, Item* item) {

    sem_wait(&cb->not_empty);

    // Leggi il valore dal buffer
    *item = buf[cb->tail]; 
    cb->tail = (cb->tail + 1) % BUFFER_SIZE;
    cb->count--;

    // Segnala che il buffer non è pieno
    sem_post(&cb->not_full);

    // random_rest(5, 10);
}

void writeItem(Buffer *buffer, Item* item, Buff which_buf) {
    buffer_t* cb = (which_buf == MAIN_BUF) ? buffer->main_buf : buffer->reverse_buf;
    Item* buf = (which_buf == MAIN_BUF) ? main_buffer : reverse_buffer;
    write_item(cb, buf, item);
}

ssize_t readItem(Buffer *buffer, Item *item, Buff which_buf) {
    switch(which_buf) {
        case MAIN_BUF:
            // Buffer in versione bloccante
            if (buffer->main_buf->count == 0) { return 0; } 
            read_item(buffer->main_buf, main_buffer, item);
            break;
        case REVERSE_BUF:
            // Ignora la lettura se il buffer è vuoto
            if (buffer->reverse_buf->count == 0) { return 0; } 
            read_item(buffer->reverse_buf, reverse_buffer, item);
            break;
    }
    return sizeof(Item);
}

void killTask(Item* item) {
    if (is_tid_stored(item->id) == 0) { return; }
    pthread_cancel(item->id);
    pthread_join(item->id, NULL);
    delete_tid(item->id);
    item->id = 0;
    pthread_mutex_unlock(&main_buf.mutex);
}