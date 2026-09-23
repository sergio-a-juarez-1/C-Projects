CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
LIBS = -lX11
TARGET = gui_password
SRC = main.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
