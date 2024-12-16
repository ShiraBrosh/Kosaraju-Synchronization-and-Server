# Compiler Settings
CXX = g++
CXXFLAGS = -Wall -g -std=c++17 -pg  # הוסף -pg כדי לתמוך ב-gprof

# Executable names
MATRIX_EXEC = graph_matrix
LIST_EXEC = graph_list

# Source files
MATRIX_SRC = adjacency_matrix/main_matrix.cpp
LIST_SRC = adjacency_list/main_list.cpp

# Headers
MATRIX_HDR = adjacency_matrix/GraphMatrix.hpp
LIST_HDR = adjacency_list/GraphList.hpp

# Default target
all: $(MATRIX_EXEC) $(LIST_EXEC)

# Build graph_matrix executable
$(MATRIX_EXEC): $(MATRIX_SRC) $(MATRIX_HDR)
	$(CXX) $(CXXFLAGS) -o $@ $(MATRIX_SRC)

# Build graph_list executable
$(LIST_EXEC): $(LIST_SRC) $(LIST_HDR)
	$(CXX) $(CXXFLAGS) -o $@ $(LIST_SRC)

# Clean up generated files
.PHONY: clean
clean:
	rm -f $(MATRIX_EXEC) $(LIST_EXEC) gmon.out matrix_gmon.out list_gmon.out matrix_profile.txt list_profile.txt

# Run profiling for graph_matrix
.PHONY: profile_matrix
profile_matrix: $(MATRIX_EXEC)
	./$(MATRIX_EXEC)
	gprof $(MATRIX_EXEC) gmon.out > matrix_profile.txt

# Run profiling for graph_list
.PHONY: profile_list
profile_list: $(LIST_EXEC)
	./$(LIST_EXEC)
	gprof $(LIST_EXEC) gmon.out > list_profile.txt
