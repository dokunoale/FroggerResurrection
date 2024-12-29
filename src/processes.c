#include "processes.h"

/**
 * @brief Creates a new buffer.
 */
Buffer newBuffer() {
    Buffer buffer;
    if (pipe(buffer.pipe_fd) == -1) {
        perror("pipe call"); exit(1);
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
void newTask(Buffer *buffer, void (*func)(int, Item), Item *item) {
    pid_t pid = fork();
    if (pid < 0) { perror("fork failed"); _exit(EXIT_FAILURE); }
    if (pid == 0) { // Child process
        close(buffer->pipe_fd[PIPE_READ]);
        item->id = getpid();
        func(buffer->pipe_fd[PIPE_WRITE], *item);
        close(buffer->pipe_fd[PIPE_WRITE]);
        _exit(EXIT_SUCCESS);
    } else { // Parent process
        close(buffer->pipe_fd[PIPE_WRITE]);
        item->id = pid;
    }
}

/**
 * Kills a task.
 * 
 * @param buffer Contains the pipe file descriptor.
 * @param item The item to be
 * @note Defined in processes.h
 */
void killTask(Buffer *buffer, Item *item) {
    close(buffer->pipe_fd[PIPE_WRITE]);
    kill(item->id, SIGKILL);
    waitpid(item->id, NULL, 0);
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
    while(write(buffer->pipe_fd[PIPE_WRITE], item, sizeof(Item)) < 0) {
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
    ssize_t bytesRead;
    do {
        bytesRead = read(buffer->pipe_fd[PIPE_READ], item, sizeof(Item));
        if (bytesRead < 0) perror("Errore nella lettura dalla pipe"); break;
        usleep(1000);
    } while (bytesRead == 0);
}

