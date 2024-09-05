#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define MAX_BUFFER_SIZE 1024

int factorial(int n) {
    if (n == 0 || n == 1) {
        return 1;
    } else {
        return n * factorial(n - 1);
    }
}

int main() {
    int serverSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);
    char buffer[MAX_BUFFER_SIZE];

    // Create UDP socket
    if ((serverSocket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
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

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        // Receive the number from the client
        ssize_t bytesRead = recvfrom(serverSocket, buffer, MAX_BUFFER_SIZE, 0, (struct sockaddr*)&clientAddr, &addrLen);

        if (bytesRead == -1) {
            perror("Recvfrom failed");
            exit(EXIT_FAILURE);
        }

        buffer[bytesRead] = '\0';

        int num = atoi(buffer);
        int result = factorial(num);

        // Send the result back to the client
        snprintf(buffer, sizeof(buffer), "%d", result);
        ssize_t bytesSent = sendto(serverSocket, buffer, strlen(buffer), 0, (struct sockaddr*)&clientAddr, addrLen);

        if (bytesSent == -1) {
            perror("Sendto failed");
            exit(EXIT_FAILURE);
        }

        printf("Received from %s:%d: Calculated factorial: %d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), result);
    }

    // Close the socket (unreachable)
    close(serverSocket);

    return 0;
}