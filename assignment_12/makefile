CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude -MMD -MP

SRC = src/main.c src/genHeap.c src/dynamicVector.c

OBJ = $(SRC:src/%.c=build/%.o)
DEP = $(OBJ:.o=.d)

TARGET = bin/app
BIN = ./$(TARGET)

all: $(TARGET)

# ===== link =====
$(TARGET): $(OBJ)
	@mkdir -p bin
	$(CC) $(OBJ) -o $(TARGET)

# ===== compile =====
build/%.o: src/%.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

# ===== run =====
run: $(TARGET)
	$(BIN)

# ===== valgrind =====
valgrind: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes $(BIN)

# ===== include dependencies =====
-include $(DEP)

# ===== clean =====
clean:
	rm -rf build bin

.PHONY: all clean valgrind run