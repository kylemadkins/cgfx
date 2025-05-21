UNAME_S := $(shell uname -s)

CC = gcc
FLAGS =
ONAME = cgfx

ifeq ($(UNAME_S), Linux)
	FLAGS = $(shell pkg-config --cflags --libs sdl2)
endif

ifeq ($(UNAME_S), Darwin)
	FLAGS = $(shell pkg-config --cflags --libs sdl2)
endif

ifeq ($(OS), Windows_NT)
	FLAGS = -Iexternal/mingw/SDL2-2.32.6/include -Lexternal/mingw/SDL2-2.32.6/lib -lmingw32 -lSDL2main -lSDL2
endif

all: build copy-dll

build:
	$(CC) -Wall -std=c99 src/*.c ${FLAGS} -o ${ONAME}

copy-dll:
ifeq ($(OS), Windows_NT)
	cp external/mingw/SDL2-2.32.6/bin/SDL2.dll .
endif

clean:
	rm ${ONAME}
