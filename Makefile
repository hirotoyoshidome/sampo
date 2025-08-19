CC = gcc

BUILD ?= debug
WARN = -Wall -Wextra -std=c17

ifeq ($(BUILD),release)
	CFLAGS = $(WARN) -Werror -O2
else
	CFLAGS = $(WARN) -g -DDEBUG
endif

LDFLAGS =

SRCDIR = src
OBJDIR = obj
BINDIR = bin

TARGET = $(BINDIR)/main

SRCS := $(wildcard $(SRCDIR)/*.c)
OBJS := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))

.PHONY:	build	clean run	cleanbuild	cleanrun	release	format	test	cleantest

.SILENT:
build: $(TARGET)

# .o -> target
$(TARGET): $(OBJS)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# .c -> .o
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(BINDIR) || true

run: build
	./$(TARGET)

cleanbuild: clean build

cleanrun: cleanbuild
	./$(TARGET)

release:
	make build BUILD=release


format:
	clang-format -i src/*.c
	clang-format -i src/*.h
	clang-format -i tests/*.c

# exlude main.o
TEST_OBJS := $(filter-out obj/main.o, $(wildcard obj/*.o))

test:
	@rm -rf tests/build || true
	@mkdir -p tests/build
	$(CC) -Wall -Wextra -std=c17 -Isrc -c tests/main.c -o tests/build/main.o
	$(CC) -Wall -Wextra -std=c17 -o tests/build/test tests/build/*.o $(TEST_OBJS)
	./tests/build/test
	@rm -rf tests/build

cleantest: cleanbuild test
