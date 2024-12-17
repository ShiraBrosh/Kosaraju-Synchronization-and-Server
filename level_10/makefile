# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -pthread

# Targets
TARGET_SERVER = graph_server
TARGET_CLIENT = graph_client

# Source files
SERVER_SRC = graph_server.cpp graph.cpp Proactor.cpp
CLIENT_SRC = graph_client.cpp graph.cpp
HEADERS = graph.hpp Proactor.hpp

# Build the server
$(TARGET_SERVER): $(SERVER_SRC) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SERVER_SRC) -o $(TARGET_SERVER)

# Build the client
$(TARGET_CLIENT): $(CLIENT_SRC) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(CLIENT_SRC) -o $(TARGET_CLIENT)

# Clean up build files
clean:
	rm -f $(TARGET_SERVER) $(TARGET_CLIENT)

# Phony targets
.PHONY: all clean

# Default target
all: $(TARGET_SERVER) $(TARGET_CLIENT)