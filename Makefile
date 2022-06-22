CC=gcc
CFLAGS=-Wall -Wextra -Wshadow -pedantic -std=c11 -ggdb -Wno-unused-parameter `pkg-config --cflags sdl2`
LIBS=`pkg-config --libs sdl2` -lm

main: main.c
	$(CC) $(CFLAGS) -o main.exe main.c $(LIBS)

