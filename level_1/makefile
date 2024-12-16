# Compiler Settings
CXX = g++  # Compiler for C++

# Compilation Flags
CXXFLAGS = -Wall -g -std=c++17

# Final executable name
EXEC_NAME = kosaraju_scc

# Source files for compilation
SOURCE_FILES = kosaraju_scc.cpp

# Object files generated from source files
OBJECT_FILES = $(SOURCE_FILES:.cpp=.o)

# Default build target
all: $(EXEC_NAME)

# Build the final executable
$(EXEC_NAME): $(OBJECT_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rule for compiling source files to object files
kosaraju_scc.o: kosaraju_scc.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -f $(EXEC_NAME) $(OBJECT_FILES)

# Run the program after building
run: $(EXEC_NAME)
	./$(EXEC_NAME)