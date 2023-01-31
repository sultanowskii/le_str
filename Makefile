CFLAGS = -Isrc
CFLAGS_DEBUG = -Isrc -g -Wall -Wextra

BUILD_FILES = $(wildcard src/*.c)

build: src/main.c $(BUILD_FILES)
	$(CC) $(CFLAGS) $^ -o liblestr.so

build-debug: src/main.c $(BUILD_FILES)
	$(CC) $(CFLAGS_DEBUG) $^ -o liblestr.so

test: src/tests/test.c $(BUILD_FILES)
	$(CC) $(CFLAGS_DEBUG) $^ -o test.elf
	./test.elf
	rm test.elf

install:
	install -m 755 liblestr.so /usr/lib/
