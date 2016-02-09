CC = gcc
CFLAGS = -g -Wall -std=gnu99

all: clean client

client: client.o
	$(CC) $(CFLAGS) -o client client.o

client.o:
	$(CC) $(CFLAGS) -c client.c

clean:
	rm -f *.o
	rm -f client
