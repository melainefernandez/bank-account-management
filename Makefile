# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -std=c++17

# Executable name
TARGET = tester

# Source files
SRCS = AVL_Database.cpp db_driver.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)

# Link object files into the final executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile each .cpp into a .o file
%.o: %.cpp AVL_Database.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean target to remove compiled files
clean:
	rm -f *.o $(TARGET)
