# Source files
SRV_SRC = graph_server.cpp graph.cpp
CLI_SRC = graph_client.cpp graph.cpp

# Object files
SRV_OBJ = $(SRV_SRC:.cpp=.o)
CLI_OBJ = $(CLI_SRC:.cpp=.o)

# Executable names
SERVER_EXEC = server_app
CLIENT_EXEC = client_app

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pthread

# Default target to build both server and client
all: $(SERVER_EXEC) $(CLIENT_EXEC)

# Build server executable
$(SERVER_EXEC): $(SRV_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Build client executable
$(CLIENT_EXEC): $(CLI_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -f $(SRV_OBJ) $(CLI_OBJ) $(SERVER_EXEC) $(CLIENT_EXEC)
