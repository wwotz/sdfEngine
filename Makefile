CC = gcc
CFLAGS = `pkg-config --cflags sdl2 SDL2_image SDL2_ttf glew` -Wall -pedantic -ggdb
CLIBS = `pkg-config --libs sdl2 SDL2_image SDL2_ttf glew` -lm
DEPS = $(wildcard include/*.h)
SRC = $(wildcard src/*.c)
OBJ = $(patsubst %.c, %.o, $(SRC))
EXEC = bin/sdfEngine

all : $(EXEC)

$(EXEC) : $(OBJ)
	$(CC) -o $@ $^ $(CLIBS)

clean:
	rm $(EXEC)
	rm src/*.o
