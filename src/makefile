CC=gcc
CFLAGS= -g -ggdb -Wall -I.
DEPS = net.h
OBJ = server.o net.o ae.o ae_epoll.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

server: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

client: client.o
	$(CC) -o $@ $^ $(CFLAGS)

all: clean server client

.PHONY: clean

clean:
	rm -rf *.o  server client
