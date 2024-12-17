#include "graph.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <sys/select.h>

#define PORT 9034
#define MAX_PENDING 10 // Number of pending connections in the queue

using namespace std;

void handleCommand(int clientSocket, const string &command, Graph &graph) {
    istringstream iss(command);
    string cmd;
    iss >> cmd;

    // Print the received command in the server's terminal
    cout << "Received command from client: " << command << endl;

    if (cmd == "Newgraph") {
        int vertexCount, edgeCount;
        iss >> vertexCount >> edgeCount;
        graph.resetGraph(vertexCount);  // Reset graph
        string response = "New graph created with " + to_string(vertexCount) + " vertices.\n";
        send(clientSocket, response.c_str(), response.size(), 0);
        
        // Printing the result on the server side
        cout << "Server: New graph created with " << vertexCount << " vertices and " << edgeCount << " edges." << endl;
    }
    else if (cmd == "Newedge") {
        int fromVertex, toVertex;
        iss >> fromVertex >> toVertex;
        graph.addEdge(fromVertex - 1, toVertex - 1);  // Adjust to 0-indexed
        string response = "Edge " + to_string(fromVertex) + " -> " + to_string(toVertex) + " added.\n";
        send(clientSocket, response.c_str(), response.size(), 0);
        
        // Printing the result on the server side
        cout << "Server: Edge " << fromVertex << " -> " << toVertex << " added." << endl;
    }
    else if (cmd == "Removeedge") {
        int fromVertex, toVertex;
        iss >> fromVertex >> toVertex;
        graph.removeEdge(fromVertex - 1, toVertex - 1);  // Adjust to 0-indexed
        string response = "Edge " + to_string(fromVertex) + " -> " + to_string(toVertex) + " removed.\n";
        send(clientSocket, response.c_str(), response.size(), 0);
        
        // Printing the result on the server side
        cout << "Server: Edge " << fromVertex << " -> " << toVertex << " removed." << endl;
    }
    else if (cmd == "Kosaraju") {
        string result = graph.printSCCs();
        send(clientSocket, result.c_str(), result.size(), 0);
        
        // Printing the result of Kosaraju on the server side
        cout << "Server: Kosaraju algorithm executed. Strongly Connected Components (SCCs):\n" << result << endl;
    }
    else {
        string response = "Error: Unknown command.\n";
        send(clientSocket, response.c_str(), response.size(), 0);
        
        // Printing the error on the server side
        cout << "Server: Error - Unknown command." << endl;
    }
}

int main() {
    int serverSocket, clientSocket;
    sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);
    fd_set master, read_fds;
    int fdmax;

    // Create server socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("Socket creation failed");
        return -1;
    }

    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, MAX_PENDING);

    FD_ZERO(&master);
    FD_ZERO(&read_fds);
    FD_SET(serverSocket, &master);
    fdmax = serverSocket;

    cout << "Server is running on port " << PORT << endl;

    while (true) {
        read_fds = master;
        if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("select");
            exit(1);
        }

        for (int i = 0; i <= fdmax; i++) {
            if (FD_ISSET(i, &read_fds)) {
                if (i == serverSocket) {
                    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrLen);
                    if (clientSocket == -1) {
                        perror("accept");
                    } else {
                        FD_SET(clientSocket, &master);
                        if (clientSocket > fdmax) fdmax = clientSocket;
                        cout << "New client connected." << endl;
                    }
                } else {
                    char buffer[1024] = {0};
                    int bytesReceived = recv(i, buffer, sizeof(buffer) - 1, 0);
                    if (bytesReceived <= 0) {
                        if (bytesReceived == 0) {
                            cout << "Client disconnected." << endl;
                        } else {
                            perror("recv");
                        }
                        close(i);
                        FD_CLR(i, &master);
                    } else {
                        string command(buffer);
                        handleCommand(i, command, *Graph::getInstance());  // Pass singleton graph
                    }
                }
            }
        }
    }

    return 0;
}
