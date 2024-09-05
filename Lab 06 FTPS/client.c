#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define PORT 1234
#define MAX_BUFFER_SIZE 1024

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[MAX_BUFFER_SIZE];
    FILE *file;

    // Create TCP socket
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Initialize server address
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Connect failed");
        exit(EXIT_FAILURE);
    }

    // Open a file for writing
    file = fopen("received.txt", "w");
    if (file == NULL) {
        perror("File open failed");
        exit(EXIT_FAILURE);
    }

    // Receive and write data to the file
    while (1) {
        ssize_t bytesRead = recv(clientSocket, buffer, MAX_BUFFER_SIZE, 0);
        if (bytesRead <= 0)
            break;

        fwrite(buffer, 1, bytesRead, file);
        printf("[+] File Received successfully!\n");
        printf("**************\n");
        fwrite(buffer, 1, bytesRead, stdout);  // Print to the terminal
        // printf("%.*s",bytesRead,buffer);
        printf("\n");
        printf("**************\n");
        printf("[+] File Printed in terminal successfully!\n");
    }
    printf("[+] Closing Client Socket\n");

    fclose(file);
    close(clientSocket);

    printf("File received successfully as 'received.txt'\n");

    return 0;
}