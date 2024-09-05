#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345
#define MAX_BUFFER_SIZE 1024
#define NUM_PACKETS 10

int main() {
    int client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket < 0) {
        perror("Error creating socket");
        exit(1);
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(SERVER_PORT);

    char buffer[MAX_BUFFER_SIZE];
    struct timeval start_time, end_time;

    while(1) {
        printf("\nEnter the message: ");
        fgets(buffer, 1024, stdin);
        gettimeofday(&start_time, NULL);
        
        sendto(client_socket, buffer, strlen(buffer), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
        int n = recvfrom(client_socket, buffer, MAX_BUFFER_SIZE, 0, NULL, NULL);

        gettimeofday(&end_time, NULL);

        long rtt = (end_time.tv_usec - start_time.tv_usec) + (end_time.tv_sec - start_time.tv_sec) * 1000000;
        printf("Received from server: %s - RTT: %ld microseconds\n", buffer, rtt);
    }
    close(client_socket);
    
    return 0;
}
