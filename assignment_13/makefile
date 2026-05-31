CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude

SRC_DIR = src
OBJ_DIR = obj

SRCS = $(SRC_DIR)/doubleLinkedList.c \
       $(SRC_DIR)/HashMap.c

OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(OBJS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

rebuild: clean all