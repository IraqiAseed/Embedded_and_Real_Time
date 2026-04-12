CC = gcc
CFLAGS = -Wall -Wextra -g

TARGET = rec.out

$(TARGET): src/main.c
	$(CC) $(CFLAGS) $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)