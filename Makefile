CC = gcc
CFLAGS = -g -Wall -std=gnu99

all: clean client server

client: client.o
	$(CC) $(CFLAGS) -o client client.o

server: server.o
	$(CC) $(CFLAGS) -o server server.o

client.o:
	$(CC) $(CFLAGS) -c client.c

server.o:
	$(CC) $(CFLAGS) -c server.c

clean:
	rm -f *.o
	rm -f client
	rm -f server
