CC = gcc

SRC_DIR = src
INC_DIR = include

CFLAGS = -I$(INC_DIR) -Wall -Wextra -g -MMD -MP

TARGET = list.out

OBJS = main.o singleLinkedlist.o

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$<

clean:
	rm -f $(TARGET) $(OBJS) *.d

-include $(OBJS:.o=.d)