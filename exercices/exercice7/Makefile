# Flexible Makefile for C++ Labs
CXX = g++
# Default flags. TP4 should add -Werror via CXXFLAGS override
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

#  Customize this target for specific data files when necessary
test: $(TARGET)
	@if [ -d "data" ] && [ "$(ls -A data 2>/dev/null)" ]; then \
		DATA_FILE=$$(find data -type f | head -n 1); \
		echo "Running with $$DATA_FILE"; \
		./$(TARGET) < "$$DATA_FILE"; \
	else \
		./$(TARGET); \
	fi

clean:
	rm -f $(OBJS) $(TARGET) output.txt build.log