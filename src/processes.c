#include "processes.h"
#include "struct.h" 

typedef struct {
    int *list;
    int length;
} PidList;

/**
 * Creates a new process with the given function and arguments.
 *
 * @param pipe_fd The file descriptor for the pipe.
 * @param pid_list The list of process IDs.
 * @param index The index of the process ID.
 * @param func The function to run in the new process.
 * @param args The arguments to pass to the function.
 */
void newTask(int *pipe_fd, PidList *pid_list, int index, void (*func)(), int *args) {
    pid_t pid = fork();
    if (pid < 0) { signal_all(*pid_list, SIGKILL); quit(); }
    if (pid == PID_CHILD) {
        close(pipe_fd[PIPE_READ]);
        func(pipe_fd[PIPE_WRITE], args);
        _exit(EXIT_SUCCESS);
    }
    pid_list->list[index] = pid;
}

void readItem (int pipe_fd, Item *item) {
    Item item;
    while( read(pipe_fd, item, sizeof(Item)) ) {
        if (item->line == 0) { break; }
    }
    
    return item;
}



// Sends a signal to all the processes
void signal_all(const PidList pids, int signal) {
    for(int i = 0; i < pids.length; i++) {
        if(pids.list[i] != 0) {  
            kill(pids.list[i], signal);
        }
    }
}
