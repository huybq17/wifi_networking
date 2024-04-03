/* -------------------------------------------------------------------
 *   Version: 1.0.0
 *   File: tcp_server_socket.c
 *   Decriptor: Contains the implementation of TCP server socket functions.
 * ------------------------------------------------------------------- */

/*
This is a simple C program illustrating a server-side socket implementation using TCP.
The server listens for incoming connections, accepts them, and sends echoed messages back to the clients.
The server runs in an infinite loop and supports multiple simultaneous connections.

Key functions utilized:
  - socket() to create a socket
  - socket address setup with sockaddr_in
  - bind() to associate the socket with the address
  - listen() to queue incoming connection requests
  - accept() to accept incoming connections
  - send() to transmit data to the clients
  - recv() to receive data from the clients
  - close() to close the socket upon completion

Errors are handled gracefully with appropriate error messages printed to the console.
*/

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>


int main(int argc, char *argv[]) {
  struct sockaddr_in client_address;
  char buffer[1024];
  int client_sock, bytes_received, bytes_sent, sock = 0;
  socklen_t client_address_size = sizeof(client_address);

  if (argc != 2) {
    printf("Usage: %s <port>\n", argv[0]);  // Input the port number as sencond argument
    return 1;
  }

  // Get server port from argument
  const int server_port = atoi(argv[1]);

  // Create a listen socket
  if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
    printf("Could not create socket\n");
    return 1;
  }

  // Set up server(local) socket address
  struct sockaddr_in server_address;
  memset(&server_address, 0, sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = INADDR_ANY;      // Listen on any IP address
  server_address.sin_port = htons(server_port);

  // Bind listen socket to the server socket address
  if (bind(sock, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
    printf("Could not bind socket\n");
    return 1;
  }

  // Listen for incoming connections
  if (listen(sock, 5) < 0) {  	                    // Queue up to 5 connection requests
    printf("Could not listen for connections\n");
    return 1;
  }
  printf("Server listening on port %d\n", server_port);

  // Loop for handling connections
  while (1) {
    // Accept incoming connection
    client_sock = accept(sock, (struct sockaddr*)&client_address, &client_address_size);
    if (client_sock < 0) {
      printf("Failed to accept connection\n");
      continue;
    }

    printf("Client connected from %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

    // Loop for communication with client
    while (1) {
      // Receive data from client
      bytes_received = recv(client_sock, buffer, sizeof(buffer), 0);
      if (bytes_received < 0) {
        printf("Failed to receive data from client\n");
        break;
      } else if (bytes_received == 0) {
        printf("Client disconnected\n");
        break;
      }

      // Echo received data back to client
      bytes_sent = send(client_sock, buffer, bytes_received, 0);
      if (bytes_sent < 0) {
        printf("Failed to send data to client\n");
        break;
      }
    }

    // Close client socket
    close(client_sock);
  }

  // Close listen socket (unreachable due to the loop)
  close(sock);

  return 0;
}
