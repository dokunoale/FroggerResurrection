#ifndef NETWORK_H
#define NETWORK_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <errno.h>

#define SLEEP_TIME 6000
#define PORT 8088
#define SERVER_IP "127.0.0.1"
#define N_CLOSE 1000

typedef struct {
    int x;
    int y;
    char symbol[2];
} Position;

int init_server();
int init_client();
int nrecv(int client_socket, int* data);
int nsend(int client_socket, int* data);
void remote();
void close_remote();

int global_client_socket;
int ndata; // buffer for network data

#endif // NETWORK_H