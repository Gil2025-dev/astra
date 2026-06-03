# =========================================================
# C Makefile for WSL2/Linux
# =========================================================

CC := gcc
TARGET := app
LEAK_TARGET := app_leak

SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)

CPPFLAGS :=
CFLAGS := -std=c17 -Wall -Wextra -g
LDFLAGS :=
LDLIBS :=
LEAK_FLAGS := 
VALGRIND := valgrind
VALGRIND_FLAGS := --leak-check=full --show-leak-kinds=all --error-exitcode=1

.PHONY: all run leak clean help

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

run: all
	./$(TARGET)

leak: clean
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LEAK_FLAGS) $(SRCS) $(LDFLAGS) -o $(LEAK_TARGET) $(LDLIBS)
	@if command -v $(VALGRIND) >/dev/null 2>&1; then \
		$(VALGRIND) $(VALGRIND_FLAGS) ./$(LEAK_TARGET); \
	else \
		./$(LEAK_TARGET); \
	fi

clean:
	rm -f $(OBJS) $(TARGET) $(LEAK_TARGET)

help:
	@echo "Usage:"
	@echo "  make       : build"
	@echo "  make run   : build and run"
	@echo "  make leak  : build and run valgrind when available"
	@echo "  make clean : remove build files"
