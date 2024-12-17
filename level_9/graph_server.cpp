#include "graph.hpp"
#include "Proactor.hpp"
#include <iostream>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <thread>
#include <mutex>
#include <vector>

#define PORT 9034
#define MAX_CLIENTS 10

/*
how to run
$ cd level_9
$ make
$ ./graph_server

on another terminal
$ nc localhost 9034


*/

Graph globalGraph(5); // Global graph instance
std::mutex graphMutex; // Mutex to protect shared graph operations

// Function to handle a single client command
void handleClient(int clientSocket) {
    string instructions = "Please insert one of the following commands:\n"
                          "Newgraph <n> <m> - Create a new graph with n vertices and m edges\n"
                          "Kosaraju - Print SCCs of the graph\n"
                          "Newedge <i> <j> - Add edge from vertex i to vertex j\n"
                          "Removeedge <i> <j> - Remove edge from vertex i to vertex j\n"
                          "Exit - to exit\n";
    send(clientSocket, instructions.c_str(), instructions.size(), 0);

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
            int vertices, edges;
            if (sscanf(buffer, "Newgraph %d %d", &vertices, &edges) != 2 || vertices <= 0 || edges < 0) {
                response = "Invalid input. Usage: Newgraph <vertices> <edges>\n";
                send(clientSocket, response.c_str(), response.size(), 0);
                continue;
            }

            {
                // Lock the graph mutex to protect the shared resource
                std::lock_guard<std::mutex> lock(graphMutex);
                globalGraph = Graph(vertices);
            }

            response = "Graph created successfully with " + std::to_string(vertices) + " vertices.\n";
            send(clientSocket, response.c_str(), response.size(), 0);
            std::cout << "Server: " << response;

            // Handle interactive edge addition
            for (int i = 0; i < edges; ++i) {
                response = "Enter edge " + std::to_string(i + 1) + " (format: <src> <dest>): ";
                send(clientSocket, response.c_str(), response.size(), 0);

                memset(buffer, 0, sizeof(buffer));
                valread = read(clientSocket, buffer, sizeof(buffer) - 1);
                if (valread <= 0) {
                    std::cout << "Error receiving edge input or client disconnected\n";
                    return;
                }

                buffer[valread] = '\0';
                int src, dest;
                if (sscanf(buffer, "%d %d", &src, &dest) != 2 || src <= 0 || dest <= 0 || src > vertices || dest > vertices) {
                    response = "Invalid edge input. Try again.\n";
                    send(clientSocket, response.c_str(), response.size(), 0);
                    --i; // Retry the current edge
                    continue;
                }

                {
                    std::lock_guard<std::mutex> lock(graphMutex);
                    if (globalGraph.addEdge(src - 1, dest - 1)) {
                        response = "Edge added successfully: " + std::to_string(src) + " -> " + std::to_string(dest) + "\n";
                        std::cout << "Server: " << response;
                    } else {
                        response = "Failed to add edge. Edge might already exist.\n";
                        std::cout << "Server: " << response;
                        --i; // Retry the current edge
                    }
                }
                send(clientSocket, response.c_str(), response.size(), 0);
            }
        } else if (receivedMessage.find("Newedge") == 0) {
            int v, w;
            if (sscanf(buffer, "Newedge %d %d", &v, &w) != 2 || v <= 0 || w <= 0) {
                response = "Invalid input. Usage: Newedge <src> <dest>\n";
                send(clientSocket, response.c_str(), response.size(), 0);
                continue;
            }

            {
                std::lock_guard<std::mutex> lock(graphMutex);
                if (globalGraph.addEdge(v - 1, w - 1)) {
                    response = "Edge added successfully: " + std::to_string(v) + " -> " + std::to_string(w) + "\n";
                    std::cout << "Server: " << response;
                } else {
                    response = "Failed to add edge. Edge might already exist.\n";
                    std::cout << "Server: " << response;
                }
            }
            send(clientSocket, response.c_str(), response.size(), 0);
        } else if (receivedMessage.find("Removeedge") == 0) {
            int v, w;
            if (sscanf(buffer, "Removeedge %d %d", &v, &w) != 2 || v <= 0 || w <= 0) {
                response = "Invalid input. Usage: Removeedge <src> <dest>\n";
                send(clientSocket, response.c_str(), response.size(), 0);
                continue;
            }

            {
                std::lock_guard<std::mutex> lock(graphMutex);
                globalGraph.removeEdge(v - 1, w - 1);
            }

            response = "Edge removed successfully: " + std::to_string(v) + " -> " + std::to_string(w) + "\n";
            std::cout << "Server: " << response;
            send(clientSocket, response.c_str(), response.size(), 0);
        } else if (receivedMessage.find("Kosaraju") == 0) {
            std::lock_guard<std::mutex> lock(graphMutex);
            response = "Running Kosaraju's algorithm...\n" + globalGraph.printSCCs();
            std::cout << "Server: " << response;
            send(clientSocket, response.c_str(), response.size(), 0);
        } else if (receivedMessage.find("Exit") == 0) {
            response = "Goodbye!\n";
            send(clientSocket, response.c_str(), response.size(), 0);
            std::cout << "Server: Client disconnected.\n";
            close(clientSocket);
            return;
        } else {
            response = "Unknown command.\n";
            send(clientSocket, response.c_str(), response.size(), 0);
            std::cout << "Server: " << response;
        }
    }
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("Setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server is running on port " << PORT << std::endl;
    Proactor proactor;

    while (true) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        if (new_socket < 0) {
            perror("Accept failed");
            continue;
        }

        proactor.startProactor(new_socket, handleClient);

        
    }

    close(server_fd);
    return 0;
}