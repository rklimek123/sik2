CXX = g++
CFLAGS = -Wall -Wextra -O2 -std=c++17

all: screen-worms-server

screen-worms-server: screen-worms-server.cpp 
	$(CXX) $(CFLAGS) -o screen-worms-server screen-worms-server.cpp client-to-server.cpp connection-manager.cpp crc32.c event-parser.cpp game-state.cpp random.cpp

clean:
	rm -f screen-worms-server
