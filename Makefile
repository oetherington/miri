SRC = miri.c
OBJ = miri.o
TARGET = miri

CC = clang
CFLAGS = -c -std=c99 -Wall -Wextra -O3 -fomit-frame-pointer \
		 -Wno-unused-parameter

LD = clang
LDFLAGS = -lm

.PHONY: all clean run runv

all: $(TARGET)

$(TARGET): $(OBJ)
	$(LD) $(LDFLAGS) $(OBJ) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	@rm -rf $(OBJ) $(TARGET)

run: $(TARGET)
	@./$(TARGET)

runv: $(TARGET)
	@valgrind ./$(TARGET)
	@rm -f ./vgcore.*
