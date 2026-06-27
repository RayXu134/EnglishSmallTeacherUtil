SOURCE = src/*.c
TARGET = main

CFLAGS =                  \
-Wall -Wextra -ggdb       \
-I include/

$(TARGET): $(SOURCE)
	gcc $^ -o $@ $(CFLAGS)

clean:
	rm -f $(TARGET) ./output.ini

asan: $(SOURCE)
	gcc $^ -o $(TARGET) $(CFLAGS) -fsanitize=address

valgrind: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET) read ./input.ini
