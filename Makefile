CC = gcc
CFLAGS = -Wall -Wextra -Iinclude $(shell pkg-config --cflags libcurl)
LDFLAGS = $(shell pkg-config --libs libcurl)

SRC = $(wildcard src/*.c)
OBJ = $(SRC:src/%.c=build/%.o)
TARGET = build/arch-update-checker

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

build/%.o: src/%.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf build/*

.PHONY: all run clean
