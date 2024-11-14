# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Itriangle_renderer

# Target executable
TARGET = face2faceindex

# Source files and object files
SRC = face2faceindex.cpp ../triangle_renderer/Cartesian3.cpp
OBJ = $(SRC:.cpp=.o)

# Build rules
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJ) $(TARGET)

# Run
run: $(TARGET)
	./$(TARGET) example.tri
