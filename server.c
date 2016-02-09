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
    char recSize[4];
  //const int recLen = 1024;
  
  int reSize = recv(socketin, recSize, sizeof(uint32_t), 0);
  int size = (int)recSize[0];
  char recBuf[size];
  int recVal = recv(socketin, recBuf, size, 0);
  printf("%s", recBuf);
  

/*  while( (recLen = recv(socketin, recBuf, sizeof(recBuf), 0))) {
    // TODO: Print input here
    printf();
  }
*/
  // TODO: clean up code
  close(socketin);
  close(socketfd);
}
