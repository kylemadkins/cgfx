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
	CFLAGS = -Iexternal/mingw/SDL2-2.32.6/include
	LDFLAGS = -Lexternal/mingw/SDL2-2.32.6/lib -lmingw32 -lSDL2main -lSDL2
	ONAME = cgfx.exe
endif

all: build copy-dll

build:
	$(CC) $(CFLAGS) src/main.c -o ${ONAME} $(LDFLAGS)

copy-dll:
ifeq ($(OS), Windows_NT)
	cp external/mingw/SDL2-2.32.6/bin/SDL2.dll .
endif

clean:
	rm ${ONAME}
