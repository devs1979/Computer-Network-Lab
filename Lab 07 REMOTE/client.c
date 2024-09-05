#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 3000
#define BUFFER_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Change this to the server's IP address

    // Connect to the server
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error connecting to server");
        exit(EXIT_FAILURE);
    }

    // Send command to the server
    printf("Enter command: ");
    fgets(buffer, sizeof(buffer), stdin);
    send(client_socket, buffer, strlen(buffer), 0);

    // Receive and print the output from the server
    while (recv(client_socket, buffer, BUFFER_SIZE, 0) > 0) {
        printf("**** Received from server ****\n");
        printf("%s", buffer);
    }

    // Close the socket
    close(client_socket);

    return 0;
}
