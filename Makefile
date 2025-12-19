# Makefile - Build system for FastIO
# Copyright (C) 2025 deotermia

CXX := g++
CXXFLAGS := -std=c++23 -O3 -Wall -Wextra -Wpedantic -I./include
AR := ar
ARFLAGS := rcs

# Targets
.PHONY: all clean test install uninstall

# Library files
LIB_SRCS := src/format.cpp src/io_utils.cpp
LIB_OBJS := $(LIB_SRCS:.cpp=.o)
STATIC_LIB := libfastio.a
SHARED_LIB := libfastio.so

# Test files
TEST_SRC := test/test_basic.cpp
TEST_EXE := test_fastio

# Example files
EXAMPLE_SRC := example.cpp
EXAMPLE_EXE := example

# Default target
all: $(STATIC_LIB) test

# Static library
$(STATIC_LIB): $(LIB_OBJS)
	$(AR) $(ARFLAGS) $@ $^

# Shared library
$(SHARED_LIB): $(LIB_OBJS)
	$(CXX) -shared $^ -o $@

# Object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -fPIC -c $< -o $@

# Tests
test: $(TEST_EXE)
	@echo "Running tests..."
	@./$(TEST_EXE)

$(TEST_EXE): $(TEST_SRC) $(STATIC_LIB)
	$(CXX) $(CXXFLAGS) $< -L. -lfastio -o $@

# Example
example: $(EXAMPLE_EXE)
	@echo "Running example..."
	@./$(EXAMPLE_EXE)

$(EXAMPLE_EXE): $(EXAMPLE_SRC) $(STATIC_LIB)
	$(CXX) $(CXXFLAGS) $< -L. -lfastio -o $@

# Clean
clean:
	rm -f $(LIB_OBJS) $(STATIC_LIB) $(SHARED_LIB) $(TEST_EXE) $(EXAMPLE_EXE)

# Install
install: $(STATIC_LIB)
	install -d /usr/local/include/fastio
	install -m 644 include/fastio/*.hpp /usr/local/include/fastio/
	install -m 644 $(STATIC_LIB) /usr/local/lib/
	install -m 644 $(SHARED_LIB) /usr/local/lib/ 2>/dev/null || true
	ldconfig

# Uninstall
uninstall:
	rm -rf /usr/local/include/fastio
	rm -f /usr/local/lib/$(STATIC_LIB) /usr/local/lib/$(SHARED_LIB)