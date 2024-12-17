#include "graph.hpp"
#include <iostream>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <thread>
#include <mutex>
#include <vector>

#define PORT 9034
#define MAX_CLIENTS 10

Graph globalGraph(5); // Global graph instance
std::mutex graphMutex; // Mutex to protect shared graph operations

// Function to handle a single client command
void handleClient(int clientSocket) {
    char buffer[1024] = {0};

    while (true) {
        // Read from client
        int valread = read(clientSocket, buffer, sizeof(buffer) - 1); // Read up to 1023 characters
        if (valread <= 0) {
            std::cout << "Client disconnected or error occurred\n";
            close(clientSocket);
            return;
        }

        // Null-terminate and print the received message
        buffer[valread] = '\0';
        std::string receivedMessage(buffer);
        std::cout << "Received from client: " << receivedMessage << std::endl;

        // Process the commands
        std::string response;
        if (receivedMessage.find("Newgraph") == 0) {
            int vertices;
            sscanf(buffer, "Newgraph %d", &vertices);

            // Lock the graph mutex to protect the shared resource
            std::lock_guard<std::mutex> lock(graphMutex);
            globalGraph = Graph(vertices);
            response = "Graph created successfully with " + std::to_string(vertices) + " vertices.\n";
            std::cout << response; // Print success message on the server
        } else if (receivedMessage.find("Newedge") == 0) {
            int v, w;
            sscanf(buffer, "Newedge %d %d", &v, &w);

            // Lock the graph mutex to protect the shared resource
            std::lock_guard<std::mutex> lock(graphMutex);
            if (globalGraph.addEdge(v - 1, w - 1)) {
                response = "Edge added successfully between vertices " + std::to_string(v) + " and " + std::to_string(w) + ".\n";
                std::cout << response; // Print success message on the server
            } else {
                response = "Failed to add edge between vertices " + std::to_string(v) + " and " + std::to_string(w) + ".\n";
                std::cout << response; // Print failure message on the server
            }
        } else if (receivedMessage.find("Removeedge") == 0) {
            int v, w;
            sscanf(buffer, "Removeedge %d %d", &v, &w);

            // Lock the graph mutex to protect the shared resource
            std::lock_guard<std::mutex> lock(graphMutex);
            globalGraph.removeEdge(v - 1, w - 1);
            response = "Edge removed successfully between vertices " + std::to_string(v) + " and " + std::to_string(w) + ".\n";
            std::cout << response; // Print success message on the server
        } else if (receivedMessage.find("Kosaraju") == 0) {
            // Lock the graph mutex to protect the shared resource
            std::lock_guard<std::mutex> lock(graphMutex);
            response = "Running Kosaraju's algorithm...\n" + globalGraph.printSCCs();
            std::cout << response; // Print SCC result on the server
        } else {
            response = "Unknown command.\n";
            std::cout << response; // Print unknown command on the server
        }

        // Send the response back to the client
        send(clientSocket, response.c_str(), response.length(), 0);
    }
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Configure socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("Setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind and listen
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("Listen");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server is running on port " << PORT << std::endl;

    while (true) {
        // Accept new connection
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        if (new_socket < 0) {
            perror("Accept failed");
            continue;
        }
        std::cout << "New connection accepted\n";

        // Handle each client in a separate thread
        std::thread clientThread(handleClient, new_socket);
        clientThread.detach(); // Detach the thread to allow independent execution
    }

    close(server_fd);
    return 0;
}
