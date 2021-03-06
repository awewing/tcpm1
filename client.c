// Alex Ewing
// Jonathan Wright
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
void sendEnd(int sock);

int main(int argc, char* argv[] ) {
  if (argc < 3) {
    printf("Error: Incorrect arguments.\n");
    printf("Usage: ./client ipAddress sport\n");
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
    }
    sendMessage(socketfd, line);
    printf("\n");
  }
  sendEnd(socketfd);
  close(socketfd);
}

void sendMessage(int sock, char *message) {
  const int32_t size = strlen(message);
  const int32_t tsize = htonl(strlen(message));
  if (size > 1025){
    printf("Message size too long\n");
    return;
  }
  if (debugflag) {
    printf("passed const size: %d\n", (int)size);
  }

  // arrange string to be sent
  unsigned char msg[sizeof(int32_t) + size + 1];
  memset(msg, '\0', sizeof(int32_t) + size + 1);

  msg[0] = (tsize >> 24) & 0xFF;
  msg[1] = (tsize >> 16) & 0xFF;
  msg[2] = (tsize >> 8) & 0xFF;
  msg[3] = tsize & 0xFF;

  memcpy(&msg[sizeof(int32_t)], message, size);

  if (debugflag) {
    printf("sending size: %d\nsending messsage: %s", *(int32_t *)msg, &msg[sizeof(int32_t)]);
  }
  send(sock, msg, sizeof(uint32_t) + size, 0);
}

void sendEnd(int sock) {
  int32_t tsize = -1;
  char msg[sizeof(int32_t)];
  memset(msg, '\0', sizeof(int32_t));
  msg[0] = (tsize >> 24) & 0xFF;
  msg[1] = (tsize >> 16) & 0xFF;
  msg[2] = (tsize >> 8) & 0xFF;
  msg[3] = tsize & 0xFF;

  if (debugflag) {
    printf("sending size: %d\n", tsize);
  }
  send(sock, msg, sizeof(int32_t), 0);
}
