#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void save_file(const char* filename, const char* content, size_t content_length) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    fwrite(content, 1, content_length, file);
    fclose(file);
}

void open_in_browser(const char* filename) {
    // Use a system command to open the default web browser
    char command[255];
    snprintf(command, sizeof(command), "xdg-open %s", filename);
    system(command);
}

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

    // Send a sample HTTP request to the server
    const char* http_request = "GET / HTTP/1.1\r\nHost: localhost\r\n\r\n";
    send(client_socket, http_request, strlen(http_request), 0);

    // Receive and print the response from the server
    ssize_t bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
    if (bytes_received > 0) {
        // Parse the response to get content length and type
        char* content_type = strstr(buffer, "Content-Type: ") + strlen("Content-Type: ");
        char* content_length_str = strstr(buffer, "Content-Length: ") + strlen("Content-Length: ");
        size_t content_length = atoi(content_length_str);
        char* content_start = strstr(buffer, "\r\n\r\n") + strlen("\r\n\r\n");

        // Save the HTML file
        save_file("received_page.html", content_start, content_length);

        // Open the HTML file in the default web browser
        open_in_browser("received_page.html");

        printf("HTML file saved as received_page.html and opened in the default web browser.\n");
    }

    // Close the socket
    close(client_socket);

    return 0;
}