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

clean:
	rm -f $(OBJS) $(TARGET) output.txt build.log