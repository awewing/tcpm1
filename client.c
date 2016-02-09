// Alex Ewing
// CSC 425
// Milestone 1
// Client

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "client.h"

void sendMessage(int sock, char* message);

int main(int argc, char* argv[] ) {
  if (argc < 3) {
    printf("Error: Incorrect arguments.\n");
    printf("Usage: ./client.c port ipAddress\n");
    exit(1);
  }

  int socketfd, funcRes;
  struct sockaddr_in serverAddr;

  socketfd = socket(AF_INET, SOCK_STREAM, 0);

  // init serverAddr parameters
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(atoi(argv[2]));
  serverAddr.sin_addr.s_addr = inet_addr(argv[1]);

  if (debugflag) {
    printf("serverIP: %s\n", inet_ntoa(serverAddr.sin_addr));
  }

  funcRes = connect(socketfd, (struct sockaddr*) &serverAddr, sizeof(serverAddr));
  if (funcRes < 0) {
    printf("Unsuccessful connect: %d\n", funcRes);
    printf("ERROR: %s\n", strerror(errno));
  }

  // Start sending data
    while (1){
      char* line = NULL;
      size_t size;
      if (getline(&line, &size , stdin) == EOF) {
          break;
      } else {
        printf("%s\n", line);
      }
        sendMessage(socketfd, line);
    }
    int32_t size = -1;
    char msg[sizeof(int32_t)];
    msg[0] = size;
    printf("sending size: %d\n", size);
    send(socketfd, msg, sizeof(int32_t), 0);

  close(socketfd);
}

void sendMessage(int sock, char *message) {
  const int32_t size = strlen(message);
  char msg[sizeof(int32_t) + size];
  msg[0] = size;
  memcpy(&msg[sizeof(int32_t)], message, size);
  printf("sending size: %d\n", size);
  send(sock, msg, sizeof(uint32_t) + size, 0);
}
