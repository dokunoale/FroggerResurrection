#include "processes.h"
#include "struct.h" 

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
 * @param pipe_fd The file descriptor for the pipe.
 * @param pid_list The list of process IDs.
 * @param index The index of the process ID.
 * @param func The function to run in the new process.
 * @param args The arguments to pass to the function.
 * @note Process version with fork()
 */
void newTask(int *pipe_fd, PidList *pid_list, int index, void (*func)(), int *args) {
    pid_t pid = fork();
    if (pid < 0) { signal_all(*pid_list, SIGKILL); _exit(EXIT_FAILURE); }
    if (pid == PID_CHILD) {
        close(pipe_fd[PIPE_READ]);
        func(pipe_fd[PIPE_WRITE], args);
        _exit(EXIT_SUCCESS);
    }
    pid_list->list[index] = pid;
}

// Writes an item to the pipe
void writeItem (int pipe_fd, Item *item) {
    while(write(pipe_fd, item, sizeof(Item)) < 0) {
        if (errno != EINTR) _exit(EXIT_FAILURE);
    }
}

// Reads an item from the pipe
void readItem (int pipe_fd, Item *item) {
    Item item;
    // while there is no item to read
    while( read(pipe_fd, item, sizeof(Item)) < 0 ) {
        if (errno != EINTR) _exit(EXIT_FAILURE);
    }
}

