#include "utils.h"

int global_client_socket;

int input(int *c) {
    int ndata;
    int temp;
    int bytes_received = nrecv(global_client_socket, &ndata); 
    if (bytes_received == 0) { 
        if ((temp = getch()) == ERR) { usleep(USLEEP); return 0; } 
        else { *c = temp; }
    }
    else { *c = ndata; }
    return 1;
};

void remote() { printf("%s\n", "Waiting the client to start..."); global_client_socket = init_server(); }

void close_remote() { 
    close(global_client_socket); 
    close_server();
}