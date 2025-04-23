# Nombre del ejecutable con carpeta destino
TARGET = bin/main

# Archivos fuente
SRC = main.c

# Compilador y flags
CC = gcc
CFLAGS = -Wall -Wextra -Wundef -Werror -Wuninitialized -Winit-self

# Regla principal
all: $(TARGET)

# Compilación
$(TARGET): $(SRC)
	@mkdir -p bin
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

# Ejecutar el binario
run: $(TARGET)
	clear && ./$(TARGET)

# Limpiar
clean:
	rm -f $(TARGET)
