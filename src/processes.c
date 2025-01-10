#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "processes.h"
#include "utils.h"

// Initializes a PID list. @note Defined in processes.c
void init_pidlist(PidList *pid_list) {
    pid_list->list = NULL;
    pid_list->length = 0;
}

// Frees the PID list. @note Defined in processes.c
void free_pidlist(PidList *pid_list) {
    free(pid_list->list);
    pid_list->list = NULL;
    pid_list->length = 0;
}

// Adds a PID to the PID list. @note Defined in processes.c
void add_pid(PidList *pid_list, pid_t pid) {
    pid_list->list = realloc(pid_list->list, (pid_list->length + 1) * sizeof(pid_t));
    if (pid_list->list == NULL) {
        perror("Failed to allocate memory for PID list");
        _exit(EXIT_FAILURE);
    }
    pid_list->list[pid_list->length] = pid;
    pid_list->length++;
}

// Removes a PID from the PID list. @note Defined in processes.c
void remove_pid(PidList *pid_list, pid_t pid) {
    int index = -1;
    for (int i = 0; i < pid_list->length; i++) {
        if (pid_list->list[i] == pid) {
            index = i;
            break;
        }
    }
    if (index != -1) {
        for (int i = index; i < pid_list->length - 1; i++) {
            pid_list->list[i] = pid_list->list[i + 1];
        }
        pid_list->list = realloc(pid_list->list, (pid_list->length - 1) * sizeof(pid_t));
        pid_list->length--;
    }
}

// Signal all processes in the PID list
void signal_all(PidList *pid_list, int signal) {
    for (int i = 0; i < pid_list->length; i++) {
        kill(pid_list->list[i], signal);
        waitpid(pid_list->list[i], NULL, 0);
    }
    free_pidlist(pid_list);
}

/**
 * Creates a new buffer.
 * 
 * @note Defined in processes.h
 */
Buffer newBuffer() {
    Buffer buffer;

    // Crea la main pipe
    if (pipe(buffer.main_pipe_fd) != 0) {
        perror("Error creating main pipe");
        _exit(EXIT_FAILURE);
    }

    // Crea la reverse pipe
    if (pipe(buffer.reverse_pipe_fd) != 0) {
        perror("Error creating reverse pipe");
        _exit(EXIT_FAILURE);
    }

    // Configura il file descriptor della reverse pipe in modalità non bloccante
    int flags = fcntl(buffer.reverse_pipe_fd[PIPE_READ], F_GETFL, 0);
    if (flags == -1) {
        perror("Error getting flags for reverse pipe");
        _exit(EXIT_FAILURE);
    }

    if (fcntl(buffer.reverse_pipe_fd[PIPE_READ], F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("Error setting non-blocking mode for reverse pipe");
        _exit(EXIT_FAILURE);
    }

    // Inizializza la lista dei PID
    init_pidlist(&buffer.pid_list);

    return buffer;
}

/**
 * Creates a new process with the given function and arguments.
 * 
 * @note Defined in processes.h
 */
void newTask(Buffer *buffer, void (*func)(Buffer, Item), Item *item) {
    pid_t pid = fork();
    if (pid < 0) { perror("fork failed"); signal_all(&buffer->pid_list, SIGKILL); _exit(EXIT_FAILURE); }
    if (pid == 0) { // Child process
        close(buffer->main_pipe_fd[PIPE_READ]); close(buffer->reverse_pipe_fd[PIPE_WRITE]);
        item->id = getpid(); 
        func(*buffer, *item); // Invoke the function
        close(buffer->main_pipe_fd[PIPE_WRITE]); close(buffer->reverse_pipe_fd[PIPE_READ]);
        _exit(EXIT_SUCCESS);
    } else { // Parent process
        item->id = pid; 
        add_pid(&buffer->pid_list, item->id);
    }
}

/**
 * Kills a task.
 * 
 * @note Defined in processes.h
 */
void killTask(Item *item) {
    if (item == NULL || item->id <= 0) { return; }
    if (kill(item->id, SIGKILL) == -1) { perror ("Error killing task"); _exit(EXIT_FAILURE); }
    if (waitpid(item->id, NULL, 0) == -1) { perror("Error waiting for task"); _exit(EXIT_FAILURE); }
    item->id = 0;
}

/**
 * Writes an item to the pipe.
 * 
 * @note Defined in processes.h
 */
void writeItem(Buffer *buffer, Item *item, Pipe pipe) {
    switch (pipe) {
        case MAIN_PIPE:
            while(write(buffer->main_pipe_fd[PIPE_WRITE], item, sizeof(Item)) < 0) {
                if(errno != EINTR) _exit(EXIT_FAILURE);
            }
            break;
        case REVERSE_PIPE:
            while(write(buffer->reverse_pipe_fd[PIPE_WRITE], item, sizeof(Item)) < 0) {
                if(errno != EINTR) _exit(EXIT_FAILURE);
            }
            break;
    }
}

/**
 * Reads an item from the pipe.
 * 
 * @note Defined in processes.h
 */
ssize_t readItem(Buffer *buffer, Item *item, Pipe pipe) {
    switch (pipe) {
        Item newitem;
        ssize_t size;

        case MAIN_PIPE: { // Leggi dalla main pipe (bloccante)
            while (read(buffer->main_pipe_fd[PIPE_READ], &newitem, sizeof(Item)) < 0) {
                usleep(3);
                if (errno != EINTR) _exit(EXIT_FAILURE);
            }
            if (sizeof(newitem) == sizeof(Item)) { size = sizeof(Item); *item = newitem; }        
        } return size;

        case REVERSE_PIPE: { // Leggi dalla reverse pipe (non bloccante)
            size = read(buffer->reverse_pipe_fd[PIPE_READ], &newitem, sizeof(Item));
            if (size < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) { return 0; } 
            else if (size == sizeof(Item)) { *item = newitem; } 
            else if (size < 0) { perror("Error reading from reverse pipe"); _exit(EXIT_FAILURE); }
        } return size;

        default: return 0;
    }
}