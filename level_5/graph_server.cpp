// graph_server.cpp

#include "graph.hpp"
#include "reactor.hpp"
#include <iostream>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

#define PORT 9034

void handleClient(int clientSocket) {
    char buffer[1024] = {0};
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int valread = read(clientSocket, buffer, 1024);
        if (valread <= 0) {
            std::cout << "Client disconnected or error occurred" << std::endl;
            break;
        }
        buffer[valread] = '\0';  // Null-terminate the string
        std::cout << "The message received from the client: " << buffer << std::endl;  // Print the received message

        // Construct a message to send back to the client
        std::string response = "The message that the server got is: ";
        response += buffer;

        // Send the response to the client
        std::cout << "hey client, i send you which messege i got\n"<< std::endl;  // Debug message for sent message
        send(clientSocket, response.c_str(), response.size(), 0);
    }

    close(clientSocket);  // Close the socket after the client disconnects or an error occurs
}


int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
       
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
       
    // Forcefully attaching socket to the port 9034
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
       
    // Binding the socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    std::cout << "Server is running on port\n" << PORT << std::endl;

    while (true) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        handleClient(new_socket);
        close(new_socket);
    }
    return 0;
}