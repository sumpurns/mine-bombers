SOURCES=src/main.cpp src/xml.cpp src/texture.cpp src/textures_reg.cpp src/helpers.cpp src/terrain.cpp src/terrain_defs.cpp src/socket.cpp
OBJECTS=$(SOURCES:.cpp=.o)
BINARY=mb

SERVER_SOURCES=src/xml.cpp src/helpers.cpp src/terrain.cpp src/terrain_defs.cpp src/socket.cpp src/server_main.cpp src/texture.cpp  src/textures_reg.cpp src/server_listner.cpp src/server_worker.cpp src/thread.cpp
SERVER_OBJECTS=$(SERVER_SOURCES:.cpp=.o)
SERVER_BINARY=mbsrv

MODULES=sdl
LIBS=`pkg-config --libs $(MODULES)`
CFLAGS=`pkg-config --cflags $(MODULES)` -I. -Wall -g -pthread
SERVER_LIBS=-pthread

.PHONY: clean
.PHONY: all
.PHONY: server
.PHONY: client
.PHONY: test

all: server client
client: $(BINARY)
server: $(SERVER_BINARY)

$(BINARY): $(OBJECTS)
	g++ $(OBJECTS) $(LIBS) -o $@

$(SERVER_BINARY): $(SERVER_OBJECTS)
	g++ $(SERVER_OBJECTS) $(LIBS) $(SERVER_LIBS) -o $@

%.o: %.cpp
	g++ -c -o $@ $< $(CFLAGS)

clean:
	rm -f $(BINARY)
	rm -f $(SERVER_BINARY)
	rm -f $(OBJECTS)
	rm -f $(SERVER_OBJECTS)
