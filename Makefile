UNAME_S := $(shell uname -s)

CC = gcc
CFLAGS =
LDFLAGS =
ONAME =

ifeq ($(UNAME_S), Linux)
	CFLAGS = -I/usr/include/SDL2
	LDFLAGS = -lSDL2
	ONAME = cgfx
endif

ifeq ($(UNAME_S), Darwin)
	CFLAGS = -I/usr/local/include/SDL2
	LDFLAGS = -lSDL2
	ONAME = cgfx
endif

ifeq ($(OS), Windows_NT)
	CFLAGS = -Iexternal/SDL2-2.32.6-mingw/include
	LDFLAGS = -Lexternal/SDL2-2.32.6-mingw/lib -lmingw32 -lSDL2main -lSDL2
	ONAME = cgfx.exe
endif

all: build copy-dll

build:
	$(CC) $(CFLAGS) main.c -o ${ONAME} $(LDFLAGS)

copy-dll:
ifeq ($(OS), Windows_NT)
	cp external/SDL2-2.32.6-mingw/bin/SDL2.dll .
endif

clean:
	rm ${ONAME}
