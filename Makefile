SOURCES = functions.c mandelbrot.c mapping.c
HEADERS = header.h

TARGET  = mandelbrot

CXX      = gcc
CXXFLAGS = -Wall -lpthread -lm

$(TARGET): $(SOURCES) 
	$(CXX) $(SOURCES) $(CXXFLAGS) -o $(TARGET)
	

.PHONY: clean
clean:
	@rm -rf $(OBJECTS) $(TARGET) core

