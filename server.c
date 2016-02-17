// Alex Ewing
// CSC 425
// Milestone 1
// Server

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdint.h>

#include "server.h"

int receiveMessage(int socketin);

int main(int argc, char* argv[]) {
  if (argc < 2) {
    printf("Error: invalid arguments\n");
    printf("Usage: ./server sport\n");
    exit(1);
  }

  int funcRes;
  struct sockaddr_in clientAddr;
  struct sockaddr_in serverAddr;

  // create socket
  int socketfd = socket(AF_INET, SOCK_STREAM, 0);

  // Init serverAddr parameters
  memset(&serverAddr, 0, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(atoi(argv[1]));
  serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

  // bind socket to serverAddr
  funcRes = bind(socketfd, (struct sockaddr*) &serverAddr, sizeof(serverAddr));
  if (funcRes < 0) {
    printf("Bind was unsuccessful: %d\n", funcRes);
    printf("ERROR: %s\n", strerror(errno));
  }

  // listen
  listen(socketfd, 5);

  // init client socket
  int socketin;
  int clientSize = sizeof(clientAddr);
  memset(&clientAddr, 0, sizeof(clientAddr));
  socketin = accept(socketfd, (struct sockaddr*) &clientAddr, (socklen_t *) &clientSize);

  if (socketin < 0) {
    printf("Accept returned an invalid socket, exiting.");
    exit(1);
  }

  // Begin receiving messages
  while (1) {
    // Recieve a message
    funcRes = receiveMessage(socketin);

    // if end of client, break
    if (funcRes < 0)
      break;

  }

  close(socketin);
  close(socketfd);
}

// receives up to several packets to construct the entire line
//  of input coming from the client side.
//  returns -1 if client closes connection
int receiveMessage(int socketin) {
  // get size of total message
  char msgSize[4];
  memset(msgSize, 0, 4);
  
  int recvSize = recv(socketin, msgSize, sizeof(int32_t), 0);
  if (debugflag) {
    printf("received size: %d\n", recvSize);
  }
  //int size = (int);
  int32_t size = *(int32_t *)msgSize;
  
  // Print size
  printf("size: %d\n", size);

  // return -1 if client signals it is exiting
  if (size == -1) {
    return -1;
  }

  // loop for the message
  int bytesLeft = size;
  char message[size];
  memset(message, '\0', size);

  while (bytesLeft > 0) {
    recvSize = recv(socketin, &message[size - bytesLeft], bytesLeft, 0);
    if (debugflag) {
      printf("received size of %d\n", recvSize);
    }

    bytesLeft -=recvSize;
  }

  // Print message
  printf("%s\n", message);
  return 0;
}
