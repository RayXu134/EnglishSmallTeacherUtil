SOURCE = src/*.c
TARGET = main

CFLAGS =                  \
-Wall -Wextra -ggdb       \
-I include/

$(TARGET): $(SOURCE)
	gcc $^ -o $@ $(CFLAGS)

asan: $(SOURCE)
	gcc $^ -o $(TARGET) $(CFLAGS) -fsanitize=address

valgrind: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET) ./input.ini
