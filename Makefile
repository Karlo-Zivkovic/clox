# Compiler and Flags
CC = gcc
CFLAGS = -Wall -Wextra -g

# Target Executable
TARGET = main

# Source Files
SRCS = main.c chunk.c memory.c debug.c value.c

# Default Rule
all: $(TARGET)

# Build Rule
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

# Clean Rule
clean:
	rm -f $(TARGET)

