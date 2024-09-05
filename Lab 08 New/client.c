#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define SERVER_IP "127.0.0.1"

int main()
{
  int client_socket;
  struct sockaddr_in server_addr;
  char buffer[1024] = {0};
  char link[1024] = {0};

  // Create socket
  client_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (client_socket == -1)
  {
    perror("Socket creation failed");
    exit(EXIT_FAILURE);
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);

  // Convert IPv4 and IPv6 addresses from text to binary form
  if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0)
  {
    perror("Invalid address/ Address not supported");
    exit(EXIT_FAILURE);
  }

  // Connect to the server
  if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
  {
    perror("Connection failed");
    exit(EXIT_FAILURE);
  }

  // Receive the link from the server
  recv(client_socket, link, sizeof(link), 0);
  printf("Received link from the server: %s\n", link);

  // Use xdg-open to open the link in the default web browser
  char command[2048]; // Increased buffer size
  snprintf(command, sizeof(command), "xdg-open %s", link);
  system(command);

  // Close the socket
  close(client_socket);

  return 0;
}
