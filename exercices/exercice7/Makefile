# Flexible Makefile for C++ Labs
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

# Test target - provide input for interactive exercises
test: $(TARGET)
	@if [ -d "data" ] && [ "$(ls -A data 2>/dev/null)" ]; then \
		DATA_FILE=$$(find data -type f | head -n 1); \
		echo "Running with $$DATA_FILE"; \
		./$(TARGET) < "$$DATA_FILE"; \
	else \
		echo "No test data found. For interactive exercises, add data/input.txt"; \
		echo "Example test input:"; \
		echo "10 0"; \
		echo "10 5"; \
		echo "10 0" | ./$(TARGET) || echo "Interactive input required"; \
	fi

clean:
	rm -f $(OBJS) $(TARGET) output.txt build.log