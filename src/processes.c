#include "processes.h"

/**
 * @brief Creates a new buffer.
 */
Buffer newBuffer() {
    Buffer buffer;
    if (pipe(buffer.main_pipe_fd) == -1) { perror("pipe call"); _exit(1); }
    if (pipe(buffer.reverse_pipe_fd) == -1) { perror("pipe call"); _exit(1); }

    int flags = fcntl(buffer.reverse_pipe_fd[PIPE_READ], F_GETFL);
    if (flags == -1) { perror("Errore durante il recupero dei flag della REVERSE_PIPE"); _exit(1); }
    if (fcntl(buffer.reverse_pipe_fd[PIPE_READ], F_SETFL, flags | O_NONBLOCK) == -1) { 
        perror("Errore durante la configurazione della REVERSE_PIPE come non bloccante"); _exit(1);
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
void newTask(Buffer *buffer, void (*func)(Buffer, Item), Item *item) {
    pid_t pid = fork();
    if (pid < 0) { perror("fork failed"); _exit(EXIT_FAILURE); }
    if (pid == 0) { // Child process
        close(buffer->main_pipe_fd[PIPE_READ]); close(buffer->reverse_pipe_fd[PIPE_WRITE]);
        item->id = getpid(); func(*buffer, *item);
        close(buffer->main_pipe_fd[PIPE_WRITE]); close(buffer->reverse_pipe_fd[PIPE_READ]);
        _exit(EXIT_SUCCESS);
    } else { // Parent process
        close(buffer->main_pipe_fd[PIPE_WRITE]);
        close(buffer->reverse_pipe_fd[PIPE_READ]);
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
    close(buffer->main_pipe_fd[PIPE_WRITE]);
    close(buffer->reverse_pipe_fd[PIPE_READ]);
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
void writeItem (Buffer *buffer, Item *item, int pipe) {
    switch (pipe) {
        case MAIN_PIPE:
            write(buffer->main_pipe_fd[PIPE_WRITE], item, sizeof(Item));
            break;
        case REVERSE_PIPE:
            write(buffer->reverse_pipe_fd[PIPE_WRITE], item, sizeof(Item));
            break;
        default:
            perror("Invalid pipe specified\n");
            break;
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
void readItem(Buffer *buffer, Item *item, int pipe) {
    ssize_t bytesRead;
    Item item_read;

    switch (pipe) {
        case MAIN_PIPE:
            // Lettura bloccante, continua finché non legge un Item
            do {
                bytesRead = read(buffer->main_pipe_fd[PIPE_READ], &item_read, sizeof(Item));
                if (bytesRead == sizeof(Item)) {
                    *item = item_read;
                    break; // Uscita dal ciclo se ha letto correttamente
                } else if (bytesRead < 0) {
                    perror("Errore nella lettura dalla MAIN_PIPE");
                    break;
                }
                // Attesa breve per evitare di occupare la CPU
                usleep(1000);
            } while (bytesRead == 0);
            break;

        case REVERSE_PIPE: {
            // Prova a leggere una sola volta
            bytesRead = read(buffer->reverse_pipe_fd[PIPE_READ], &item_read, sizeof(Item));
            if (bytesRead == sizeof(Item)) {
                *item = item_read; // Lettura riuscita
            } else if (bytesRead < 0) {
                if (errno != EAGAIN && errno != EWOULDBLOCK) {
                    perror("Errore nella lettura dalla REVERSE_PIPE");
                }
                // Se errno è EAGAIN o EWOULDBLOCK, non c'è nulla da leggere: procediamo
            }
            break;
        }

        default:
            fprintf(stderr, "Invalid pipe specified\n");
            return;
    }
}

