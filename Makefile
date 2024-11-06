CXX = g++
CXXFLAGS = -Iinclude -Wall -std=c++23

SRC = src/server.cpp src/client.cpp src/main.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = semaphore

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
clean:
	rm -f $(OBJ) $(TARGET)
