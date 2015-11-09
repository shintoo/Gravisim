CC=gcc
CFLAGS=-std=c11 -Wall -g
LDFLAGS=-lm -lSDL2 -lSDL2_image
SRC=src/main.c src/particle.c

all:
	$(CC) $(CFLAGS) $(SRC) $(LDFLAGS) -o gravisim
