# Nombre del servidor y el compilador
SERVER = Servidor/server
CC = gcc

# Opciones de compilacion
CFLAGS = -pthread

# Fuentes y objetos
SRC = Servidor/server.c

# Objetivos
all: $(SERVER)

$(SERVER): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

run: $(SERVER)
	./$(SERVER) 8080 logFile

clean:
	rm -f $(SERVER) $(OBJ)

.PHONY: all clean run
