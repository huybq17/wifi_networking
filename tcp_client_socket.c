/* -------------------------------------------------------------------
 *   Version: 1.0.0
 *   File: tcp_client_socket.c
 *   Decriptor: Contains the implementation of TCP client socket functions.
 * --------------------------------------------------------------------*/

/*
This is a simple C program illustrating a client-side socket connection to a server.
The client can send messages to the server and receive echoed messages in return.
The program supports sending messages until the user types "quit" and presses Enter.

Key functions utilized:
  - socket() to create a socket
  - socket address setup with sockaddr_in
  - connect() to connect to the server
  - send() to transmit data to the server
  - recv() to receive data from the server
  - close() to close the socket upon completion

Errors are handled gracefully with appropriate error messages printed to the console.
*/

#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
  const char* server_name = "127.0.0.1";  // Replace with server IP if needed
  const int server_port = 8080;             // Replace with server port

  // Create a socket
  int sock;
  if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
    printf("Could not create socket\n");
    return 1;
  }

  // Set up remote (server) socket address
  struct sockaddr_in server_address;
  memset(&server_address, 0, sizeof(server_address));
  server_address.sin_family = AF_INET;
  inet_pton(AF_INET, server_name, &server_address.sin_addr);  // Server IP address
  server_address.sin_port = htons(server_port);  // Server port number

  // Connect to server
  if (connect(sock, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
    printf("Could not connect to server!\n");
    return 1;
  }

  // Loop for sending and receiving data
  char buffer[1024];
  while (1) {
    // Get message from user
    printf("Enter message to send (or 'quit' to exit): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
      perror("fgets");
      break;
    }

    // Check for quit command
    if (strncmp(buffer, "quit", 4) == 0) {
      break;
    }

    // Send message to server
    int bytes_sent = send(sock, buffer, strlen(buffer), 0);
    if (bytes_sent < 0) {
      printf("Failed to send data\n");
      break;
    }

    // Receive echo from server
    int bytes_received = recv(sock, buffer, sizeof(buffer), 0);
    if (bytes_received < 0) {
      printf("Failed to receive data\n");
      break;
    } else if (bytes_received == 0) {
      printf("Server disconnected\n");
      break;
    }

    // Print received echo
    printf("Received: %s\n", buffer);
  }

  // Close socket
  close(sock);

  return 0;
}

