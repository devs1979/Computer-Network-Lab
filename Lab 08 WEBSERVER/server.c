// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void send_response(int client_socket, const char* status, const char* content_type, const char* content, size_t content_length) {
    char response[BUFFER_SIZE];
    sprintf(response, "HTTP/1.1 %s\r\nContent-Type: %s\r\nContent-Length: %zu\r\n\r\n%s", status, content_type, content_length, content);
    send(client_socket, response, strlen(response), 0);
}

void handle_request(int client_socket, const char* request) {
    char method[10];
    char path[255];

    // Parse the request
    sscanf(request, "%s %s", method, path);

    // Default to index.html if no specific page is requested
    if (strcmp(path, "/") == 0) {
        strcpy(path, "/index.html");
    }

    // Open the requested file
    FILE* file = fopen(path + 1, "rb");  // Ignore the leading '/'
    if (file == NULL) {
        send_response(client_socket, "404 Not Found", "text/html", "<h1>404 Not Found</h1>", 0);
    } else {
        // Get the file size
        fseek(file, 0, SEEK_END);
        size_t file_size = ftell(file);
        fseek(file, 0, SEEK_SET);

        // Read the file content
        char* file_content = (char*)malloc(file_size);
        fread(file_content, 1, file_size, file);

        // Send the response with the file content
        send_response(client_socket, "200 OK", "text/html", file_content, file_size);

        // Clean up
        free(file_content);
        fclose(file);
    }
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error binding");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, 10) == -1) {
        perror("Error listening");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        // Accept a connection
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_size);
        if (client_socket == -1) {
            perror("Error accepting connection");
            exit(EXIT_FAILURE);
        }

        // Receive the request from the client
        recv(client_socket, buffer, BUFFER_SIZE, 0);
        printf("Received request:\n%s\n", buffer);

        // Handle the request
        handle_request(client_socket, buffer);

        // Close the client socket
        close(client_socket);
    }

    // Close the server socket
    close(server_socket);

    return 0;
}
