#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int server_fd, client1_fd, client2_fd;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    
    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    // Attach socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("Setsockopt failed");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    // Bind the socket to the specified IP and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    
    // Listen for client connections
    if (listen(server_fd, 2) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
    
    printf("Server is waiting for connections...\n");
    
    // Accept first client connection
    if ((client1_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }
    
    printf("First client connected\n");
    
    // Accept second client connection
    if ((client2_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }
    
    printf("Second client connected\n");
    
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        
        // Receive data from client 1
        recv(client1_fd, buffer, sizeof(buffer), 0);
        printf("Client 1: %s", buffer);
        
        // Send data from client 1 to client 2
        send(client2_fd, buffer, strlen(buffer), 0);
        
        memset(buffer, 0, sizeof(buffer));
        
        // Receive data from client 2
        recv(client2_fd, buffer, sizeof(buffer), 0);
        printf("Client 2: %s", buffer);
        
        // Send data from client 2 to client 1
        send(client1_fd, buffer, strlen(buffer), 0);
    }
    
    return 0;
}

