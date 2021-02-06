
#OS detection
ifeq ($(OS),Windows_NT) 
detected_OS := Windows
else
detected_OS := $(shell sh -c 'uname 2>/dev/null || echo Unknown')
endif

CC=gcc
INCLUDES= bmpio/include

ifeq ($(detected_OS),Windows)
LIBS = -lm lib/*.c bmpio/windows_x86_64/lib/libfcp.a
main : main.c lib/*.c
	$(CC) -Wall -static-libgcc -I$(INCLUDES) $(LIBS)  main.c -o main.exe
endif

ifeq ($(detected_OS),Linux)
LIBS = -lm lib/*.c bmpio/linux_x86_64/lib/libfcp.a
main : main.c lib/*.c
	$(CC) -Wall -static-libgcc -I$(INCLUDES) $(LIBS) main.c -o main
endif
