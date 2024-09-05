#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define PORT 12345
#define MAX_BUFFER_SIZE 1024

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[MAX_BUFFER_SIZE];
    int num;


    // Create UDP socket
    if ((clientSocket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Initialize server address
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    while(1){
        printf("Enter a number to calculate factorial: ");
        scanf("%d", &num);

        if(num<0){
            printf("Please Enter a number greater than 0\n");
            continue;
        }
        // Send the number to the server
        snprintf(buffer, sizeof(buffer), "%d", num);
        ssize_t bytesSent = sendto(clientSocket, buffer, strlen(buffer), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

        if (bytesSent == -1) {
            perror("Sendto failed");
            exit(EXIT_FAILURE);
        }

        // Receive the result from the server
        ssize_t bytesRead = recvfrom(clientSocket, buffer, MAX_BUFFER_SIZE, 0, NULL, NULL);

        if (bytesRead == -1) {
            perror("Recvfrom failed");
            exit(EXIT_FAILURE);
        }

        buffer[bytesRead] = '\0';

        printf("Factorial of %d is: %s\n", num, buffer);
    }

    // Close the socket
    close(clientSocket);

    return 0;
}