CXX      = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
TARGET   = campusmate
SRC      = src/main.cpp

# Detect OS for Windows-specific linker needs
ifeq ($(OS),Windows_NT)
    TARGET := campusmate.exe
endif

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(TARGET).exe

.PHONY: all run clean
