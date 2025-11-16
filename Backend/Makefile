CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -pthread
TARGET = server
SOURCES = main.cpp Graph.cpp User.cpp HTTPParser.cpp JSONParser.cpp
OBJECTS = $(SOURCES:.cpp=.o)

ifeq ($(OS),Windows_NT)
    LDFLAGS = -lws2_32 -pthread
    RM = del /Q
    TARGET = server.exe
else
    LDFLAGS = -pthread
    RM = rm -f
endif

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJECTS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
