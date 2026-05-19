CC = gcc

CFLAGS = -Wall -Wextra -g -fPIC -Iinclude

LDFLAGS = -shared

SRC = $(wildcard src/*.c)

OBJ = $(patsubst src/%.c, build/%.o, $(SRC))

LIB = lib/libds.so

TARGET = bin/app


all: directories $(LIB)


directories:
	mkdir -p build lib bin


$(LIB): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^


build/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@


app: all
	$(CC) -Wall -Wextra -g -Iinclude main.c -Llib -lds -o $(TARGET)


run: app
	LD_LIBRARY_PATH=lib ./$(TARGET)


clean:
	rm -rf build/*.o lib/*.so bin/*


.PHONY: all directories app run clean