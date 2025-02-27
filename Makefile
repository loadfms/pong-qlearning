CC = g++
CFLAGS = -Wall -Iinclude  # Add include folder for header files
LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

all: my_game

my_game: main.cpp AI.cpp QLearning.cpp
	$(CC) $(CFLAGS) -o my_game main.cpp AI.cpp QLearning.cpp $(LDFLAGS)

clean:
	rm -f my_game

run: my_game
	./my_game
