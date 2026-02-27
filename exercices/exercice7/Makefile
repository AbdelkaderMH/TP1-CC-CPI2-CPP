# Flexible Makefile for C++ Labs (CI-Friendly)
CXX = g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -pedantic
SRCS = $(wildcard src/*.cpp)
OBJS = $(SRCS:.cpp=.o)
TARGET = main

# Check if include directory exists before adding flag
ifneq ($(wildcard include/),)
    INCLUDES = -I./include
else
    INCLUDES = 
endif

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Test target - CI-friendly (no interactive input)
test: $(TARGET)
	@echo "=== Running Tests ==="
	@echo "✓ Providing default test input"
	@echo "10 5" | ./$(TARGET) > output.txt
	@echo "✓ Test completed. Output saved to output.txt"
	cat output.txt

clean:
	rm -f $(OBJS) $(TARGET) output.txt build.log