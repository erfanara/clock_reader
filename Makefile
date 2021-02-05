
#OS detection
ifeq ($(OS),Windows_NT) 
detected_OS := Windows
else
detected_OS := $(shell sh -c 'uname 2>/dev/null || echo Unknown')
endif


ifeq ($(detected_OS),Windows)
CC = gcc
LIBS = -lm lib/*.c bmpio/windows_x86_64/lib/libfcp.a
main : main.c
	$(CC) -Wall -static-libgcc $(LIBS) `pkg-config --cflags gtk4` `pkg-config --libs gtk4` main.c -o main
endif

ifeq ($(detected_OS),Linux)
CC = gcc
LIBS = -lm lib/*.c bmpio/linux_x86_64/lib/libfcp.a
main : main.c
	$(CC) -Wall -static-libgcc $(LIBS) `pkg-config --cflags gtk4` `pkg-config --libs gtk4` main.c -o main
endif
