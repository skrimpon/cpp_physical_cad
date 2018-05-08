##
# Panagiotis Skrimponis

CC = g++
CC_FLAGS =  -O3 -std=c++11 `pkg-config --cflags gtk+-2.0` `pkg-config --cflags gthread-2.0`
LD_FLAGS = `pkg-config --libs gtk+-2.0` `pkg-config --libs gthread-2.0` -lreadline -lm -ltcl -L/usr/local/lib

EXEC = main
SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(patsubst src/%,build/%,$(SOURCES:.cpp=.o))
INCLUDE = -I/opt/local/include/cairo/ -I/opt/local/include/ -I/opt/local/include/gtk-2.0/ -Iinclude

all: build

build: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC) $(LD_FLAGS)

build/%.o: src/%.cpp
	$(CC) -c $(INCLUDE) $(CC_FLAGS) $< -o $@

clean:
	rm -rf $(EXEC) $(OBJECTS)

run: build
	./$(EXEC)