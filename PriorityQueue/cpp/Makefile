# =========================================================
# C++ Makefile for WSL2/Linux
# =========================================================

CXX := g++
TARGET := app
LEAK_TARGET := app_leak

SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:.cpp=.o)

CPPFLAGS :=
CXXFLAGS := -std=c++2a -Wall -Wextra -g
LDFLAGS :=
LDLIBS :=
LEAK_FLAGS :=
VALGRIND := valgrind
VALGRIND_FLAGS := --leak-check=full --show-leak-kinds=all --error-exitcode=1

.PHONY: all run leak clean help

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)

%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

run: all
	./$(TARGET)

leak: clean
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LEAK_FLAGS) $(SRCS) $(LDFLAGS) -o $(LEAK_TARGET) $(LDLIBS)
	@if command -v $(VALGRIND) >/dev/null 2>&1; then \
		$(VALGRIND) $(VALGRIND_FLAGS) ./$(LEAK_TARGET); \
	else \
		./$(LEAK_TARGET); \
	fi

clean:
	rm -f $(OBJS) $(TARGET) $(LEAK_TARGET) app.exe

help:
	@echo "Usage:"
	@echo "  make       : build"
	@echo "  make run   : build and run"
	@echo "  make leak  : build and run valgrind when available"
	@echo "  make clean : remove build files"
