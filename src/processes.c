#include "processes.h"
#include "struct.h" 
#include "utils.h"

// Sends a signal to all the processes
void signal_all(const PidList pids, int signal) {
    for(int i = 0; i < pids.length; i++) {
        if(pids.list[i] != 0) {  
            kill(pids.list[i], signal);
        }
    }
}

/**
 * @brief Creates a new buffer.
 */
Buffer newBuffer() {
    Buffer buffer;
    if (pipe(buffer.pipe_fd) == -1) {
        perror("pipe call"); endwin(); exit(1);
    }
    return buffer;
}

/**
 * Creates a new process with the given function and arguments.
 * 
 * @param buffer Contains the pipe file descriptor.
 * @param func The function to be executed by the process.
 * @param item The item to be passed to the function; contains the process id.
 * @note Defined in processes.h
 */
void newTask(Buffer* buffer, void (*func)(), Item* item) {
    pid_t pid = fork();
    if (pid < 0) { _exit(EXIT_FAILURE); } // TODO: error handling
    if (pid == PID_CHILD) {
        close(buffer->pipe_fd[PIPE_READ]);
        item->id = getpid();
        func(buffer->pipe_fd[PIPE_WRITE], *item);
        _exit(EXIT_SUCCESS);
    }
}

/**
 * Writes an item to the pipe.
 * 
 * @param buffer Contains the pipe file descriptor.
 * @param item The item to be written to the pipe.
 * 
 * @note Defined in processes.h
 */
void writeItem (Buffer* buffer, Item *item) {
    while(write(buffer->pipe_fd, item, sizeof(Item)) < 0) {
        if (errno != EINTR) _exit(EXIT_FAILURE);
    }
}

/**
 * Reads an item from the pipe.
 * 
 * @param buffer Contains the pipe file descriptor.
 * @param item The item to be read from the pipe.
 * 
 * @note Defined in processes.h
 */
void readItem (Buffer* buffer, Item *item) {
    Item item;
    // while there is no item to read
    while( read(buffer->pipe_fd, item, sizeof(Item)) < 0 ) {
        if (errno != EINTR) _exit(EXIT_FAILURE);
    }
}

