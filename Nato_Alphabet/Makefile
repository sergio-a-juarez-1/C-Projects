# Compiler configuration
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2

# Driver Linker Flags for Raylib, OpenGL, and X11 graphic environments
LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# Name of your source file and compiled output program binary
TARGET = gui_nato
SRC = main.c

# Default execution route (Runs when you just type 'make')
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

# Cleanup helper script routine (Runs when you type 'make clean')
clean:
	rm -f $(TARGET)

.PHONY: all clean
