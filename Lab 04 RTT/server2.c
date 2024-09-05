#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_PORT 12345
#define MAX_BUFFER_SIZE 1024

int main() {
    int server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket < 0) {
        perror("Error creating socket");
        exit(1);
    }

    struct sockaddr_in server_addr, client_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error binding");
        close(server_socket);
        exit(1);
    }

    char buffer[MAX_BUFFER_SIZE];
    socklen_t client_len = sizeof(client_addr);

    while (1) {
        int n=recvfrom(server_socket, buffer, MAX_BUFFER_SIZE, 0, (struct sockaddr*)&client_addr, &client_len);
        printf("\nCiient Message: %s\n",buffer);
        sendto(server_socket, buffer, n, 0, (struct sockaddr*)&client_addr, client_len);
    }
    close(server_socket);
    
    return 0;
}
