SOURCES=src/main.cpp src/xml.cpp src/texture.cpp src/textures_reg.cpp src/helpers.cpp src/terrain.cpp src/terrain_defs.cpp
OBJECTS=$(SOURCES:.cpp=.o)

MODULES=sdl
LIBS=`pkg-config --libs $(MODULES)`
CFLAGS=`pkg-config --cflags $(MODULES)` -I. -Wall -g

.PHONY: clean
.PHONY: all
.PHONY: test

all: $(OBJECTS)
	g++ $(OBJECTS) $(LIBS) -o mb

%.o: %.cpp
	g++ -c -o $@ $< $(CFLAGS)

clean:
	rm -f mb
	rm -f $(OBJECTS)
