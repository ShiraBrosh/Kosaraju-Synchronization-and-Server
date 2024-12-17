# Compiler settings
CXX = g++  # Compiler
CXXFLAGS = -std=c++17 -Wall -Wextra -pthread  # Compilation flags

# Executable targets
SERVER_EXEC = server_app
CLIENT_EXEC = client_app

# Source files for each target
SRV_SRC = graph_server.cpp graph.cpp reactor.cpp
CLI_SRC = graph_client.cpp reactor.cpp

# Object files generated from source files
SRV_OBJ = $(SRV_SRC:.cpp=.o)
CLI_OBJ = $(CLI_SRC:.cpp=.o)

# Build all executables by default
all: $(SERVER_EXEC) $(CLIENT_EXEC)

# Build server executable
$(SERVER_EXEC): $(SRV_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Build client executable
$(CLIENT_EXEC): $(CLI_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile source files individually
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(SERVER_EXEC) $(CLIENT_EXEC) $(SRV_OBJ) $(CLI_OBJ)

# Run server
run_server: $(SERVER_EXEC)
	./$(SERVER_EXEC)

# Run client
run_client: $(CLIENT_EXEC)
	./$(CLIENT_EXEC)

# Phony targets to avoid conflicts with files
.PHONY: all clean run_server run_client