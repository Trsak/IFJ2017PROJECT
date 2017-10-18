TARGET = ifj2017
LIBS = -lm
CC = gcc

SRC_DIR := ./src

CFLAGS=-std=c99
CFLAGS += -Wall
CFLAGS += -Wextra

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(wildcard $(SRC_DIR)/*.c)
HEADERS = $(wildcard $(SRC_DIR)/*.h)

%.o: %.c $(HEADERS)
    $(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
    $(CC) $(OBJECTS) -Wall $(LIBS) -o $@

clean:
    -rm -f *.o
    -rm -f $(TARGET)