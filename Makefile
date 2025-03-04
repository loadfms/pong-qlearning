CC = g++
CFLAGS = -Wall -Iinclude  # Add include folder for header files
LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

all: server client  # Build both server and client

server: main.cpp 
	$(CC) $(CFLAGS) -o server main.cpp $(LDFLAGS)

client: client.cpp
	$(CC) $(CFLAGS) -o client client.cpp $(LDFLAGS)

clean:
	rm -f server client

run-server: server
	./server

run-client: client
	./client
