#include "utils.h"

int input(int *c) {
    int temp;
    int bytes_received = nrecv(global_client_socket, &ndata); 
    if (bytes_received == 0) { 
        if ((temp = getch()) == ERR) { usleep(USLEEP); return 0; } 
        else { *c = temp; }
    }
    else { *c = ndata; }
    return 1;
};
