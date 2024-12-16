# Compiler Settings
CXX = g++  # Compiler for C++
CXXFLAGS = -Wall -g -std=c++17 -pg  # הוסף -pg כדי לתמוך ב-gprof

# Final executable names
LIST_EXEC = kosaraju_list
DEQUE_EXEC = kosaraju_deque

# Source file
SOURCE_FILES = kosaraju_scc.cpp

# Build both versions by default
all: $(LIST_EXEC) $(DEQUE_EXEC)

# Build for list
$(LIST_EXEC): $(SOURCE_FILES)
	$(CXX) $(CXXFLAGS) -DUSE_LIST -o $@ $^

# Build for deque
$(DEQUE_EXEC): $(SOURCE_FILES)
	$(CXX) $(CXXFLAGS) -DUSE_DEQUE -o $@ $^

# Clean up generated files
.PHONY: clean
clean:
	rm -f $(LIST_EXEC) $(DEQUE_EXEC) gmon.out list_gmon.out deque_gmon.out list_profile.txt deque_profile.txt kosaraju_list.o kosaraju_deque.o


# Run profiling for list
.PHONY: profile_list
profile_list: $(LIST_EXEC)
	./$(LIST_EXEC)
	gprof $(LIST_EXEC) gmon.out > list_profile.txt

# Run profiling for dequecle