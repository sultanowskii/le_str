CFLAGS       = -fPIC
CFLAGS_DEBUG = -g -Wall -Wextra -fPIC
LDFLAGS      = -shared
INCLUDES     = -Isrc -L.

SRCS         = $(wildcard src/*.c)
0BJS         = $(SRCS:.c=.o) # simply relpacing .c with .o

TARGET_BASIC = lestr
TARGET       = lib$(TARGET_BASIC).so

all: $(TARGET)

clean:
	rm -rf $(TARGET) **/*.o

$(TARGET): $(0BJS)
	$(CC) $(INCLUDES) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $^

test: src/tests/test.c $(0BJS)
	$(CC) $(INCLUDES) $(CFLAGS_DEBUG) -o test.elf $^
	./test.elf
	rm test.elf

test-shared: src/tests/test.c $(TARGET)
	$(CC) $(INCLUDES) $(CFLAGS_DEBUG) -l$(TARGET_BASIC) -o test.elf $<
	LD_LIBRARY_PATH=. ./test.elf
	rm test.elf

# using some automatic variables, which is pretty convenient
%.o: %.c
	$(CC) $(INCLUDES) $(CFLAGS) -c -o $@ $<

install:
	install -m 755 $(TARGET) /usr/local/lib/
