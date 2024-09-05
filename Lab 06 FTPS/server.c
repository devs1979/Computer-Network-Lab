#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 1234
#define FILENAME "file.txt"
#define MAX_BUFFER_SIZE 1024

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);
    char buffer[MAX_BUFFER_SIZE];
    FILE *file;

    // Create TCP socket
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Initialize server address
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket to server address
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(serverSocket, 5) == -1) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    // Accept client connection
    if ((clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrLen)) == -1) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    // Open and send the file
    file = fopen(FILENAME, "r");
    if (file == NULL) {
        perror("File open failed");
        exit(EXIT_FAILURE);
    }

    while (1) {
        size_t bytesRead = fread(buffer, 1, MAX_BUFFER_SIZE, file);
        if (bytesRead <= 0)
            break;

        ssize_t bytesSent = send(clientSocket, buffer, bytesRead, 0);
        if (bytesSent == -1) {
            perror("Send failed");
            exit(EXIT_FAILURE);
        }
        printf("[+] File sent successfully!\n");
    }
    printf("[+] Closing Server Socket\n");
    fclose(file);
    close(clientSocket);
    close(serverSocket);

    return 0;
}