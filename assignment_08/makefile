CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -g

TARGET = app

OBJS = main.o dynamicVector.o dynamicStack.o circularQueue.o

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

main.o: src/main.c
	$(CC) $(CFLAGS) -c $< -o $@

dynamicVector.o: src/dynamicVector.c
	$(CC) $(CFLAGS) -c $< -o $@

dynamicStack.o: src/dynamicStack.c
	$(CC) $(CFLAGS) -c $< -o $@

circularQueue.o: src/circularQueue.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) *.o