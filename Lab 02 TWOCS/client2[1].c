#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int client_fd;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    
    // Create socket file descriptor
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }
    
    // Connect to the server
    if (connect(client_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }
    
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        // Receive data from server (Client 1)
        recv(client_fd, buffer, sizeof(buffer), 0);
        printf("Client 1 to Client 2: %s", buffer);
        
        memset(buffer, 0, sizeof(buffer));
        // Send data to server (Client 2)
        printf("Client 2, Enter your message: ");
        fgets(buffer, sizeof(buffer), stdin);
        send(client_fd, buffer, strlen(buffer), 0);
    }
    
    return 0;
}

