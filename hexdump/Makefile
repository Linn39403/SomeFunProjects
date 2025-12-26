# Define the compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11

# Define the executable name and source file
TARGET = hexdump
SOURCE = hexdump.c

# The default target (what runs when you just type 'make')
all: $(TARGET)

# Rule to build the executable from the source file
$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) $(SOURCE) -o $(TARGET)

# Clean up the built files
.PHONY: clean
clean:
	rm -f $(TARGET)
