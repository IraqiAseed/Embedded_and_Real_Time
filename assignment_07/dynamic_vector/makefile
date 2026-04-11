CC = gcc

CFLAGS = -g -Wall -Wextra -MMD -MP -Iinclude -Itests
PICFLAGS = -fPIC

TARGET = bin/vector_tests
LIB_DIR = lib
LIB = $(LIB_DIR)/libdynamicVector.so

LIB_OBJ = build/dynamicVector.o
MAIN_OBJ = build/main.o
TEST_OBJ = build/tests.o

OBJ = $(LIB_OBJ) $(MAIN_OBJ) $(TEST_OBJ)
DEP = $(OBJ:.o=.d)

.PHONY: all clean run dirs

all: $(TARGET)

$(TARGET): $(MAIN_OBJ) $(TEST_OBJ) $(LIB) | dirs
	$(CC) $(MAIN_OBJ) $(TEST_OBJ) -L$(LIB_DIR) -ldynamicVector -o $@

$(LIB): $(LIB_OBJ) | dirs
	$(CC) -shared -o $@ $^

build/dynamicVector.o: src/dynamicVector.c | dirs
	$(CC) $(CFLAGS) $(PICFLAGS) -c $< -o $@

build/main.o: src/main.c | dirs
	$(CC) $(CFLAGS) -c $< -o $@

build/tests.o: tests/tests.c | dirs
	$(CC) $(CFLAGS) -c $< -o $@

dirs:
	mkdir -p build bin lib

-include $(DEP)

run: $(TARGET)
	LD_LIBRARY_PATH=./lib ./$(TARGET)

clean:
	rm -rf build bin lib