#include "network.h"
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h> // inet_addr
#include <fcntl.h>

int server_socket;

int set_nonblocking(int sockfd) {
    int flags = fcntl(sockfd, F_GETFL, 0);
    if (flags == -1) {
        return -1;
    }
    return fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
}

int init_server() {
    // Create the socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Bind the socket
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(server_socket, 5) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // Accept a connection
    struct sockaddr_in client_address;
    socklen_t client_address_size = sizeof(client_address);
    int client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_size);
    if (client_socket == -1) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    set_nonblocking(client_socket);
    return client_socket;
}

int init_client() {
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = inet_addr(SERVER_IP);
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    set_nonblocking(client_socket);
    return client_socket;
}

int nrecv(int client_socket, int* data) {
    int bytes_received = recv(client_socket, data, sizeof(int), 0);
    if (bytes_received == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) { return 0; }
        else { exit(EXIT_FAILURE); }
    }
    return bytes_received;
}

int nsend(int client_socket, int* data) {
    int bytes_sent = send(client_socket, data, sizeof(int), 0);
    if (bytes_sent == -1) {
        perror("send");
        return 0;
    }
    return bytes_sent;
}

int close_server() {
    if (close(server_socket) == -1) {
        perror("close");
        return -1;
    }
    return 0;
}