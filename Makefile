# Makefile

CC = gcc
CFLAGS = -Wall -Wextra -std=c99
SRC = src/main.c src/mapa.c src/jugadores.c src/io.c src/rutas.c
OBJ = $(SRC:.c=.o)
EXEC = navegacion_robotica

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)

.PHONY: all clean