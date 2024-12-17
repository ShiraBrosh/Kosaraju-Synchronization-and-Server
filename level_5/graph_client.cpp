// graph_client.cpp
#include <iostream>
#include <sys/socket.h>
#include <string>
#include <sstream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 9034

using namespace std;

void sendToServer(int socket, const string &msg) {
    cout << "DEBUG: Sending message to server: " << msg << endl;  
    if (send(socket, msg.c_str(), msg.size(), 0) == -1) {
        cerr << "Failed to send message to server.\n";
    }
}

string getResponseFromServer(int socket) {
    char buffer[1024];
    ssize_t received = recv(socket, buffer, sizeof(buffer) - 1, 0);
    if (received <= 0) {
        return "Server disconnected or error receiving response.\n";
    }
    buffer[received] = '\0';
    string response(buffer);
    cout << "DEBUG: Message received from server: " << response << endl;  // Debug message for received message
    return response;
}


int main() {
    cout << "Starting client..." << endl;

    int clientSocket;
    sockaddr_in serverAddr;

    // Create a socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        perror("Error creating socket");
        return -1;
    }

    // Set server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        close(clientSocket);
        return -1;
    }

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Connection failed");
        close(clientSocket);
        return -1;
    }

    // Display server's initial response
    cout << getResponseFromServer(clientSocket) << endl;

    string inputLine;

    while (true) {
        cout << "> ";  // Prompt for user input
        if (!getline(cin, inputLine)) {
            cout << "Input error, closing client.\n";
            break;
        }

        sendToServer(clientSocket, inputLine + "\n");

        if (inputLine.rfind("Newgraph", 0) == 0) {
            istringstream inputParser(inputLine);
            string command;
            int vertices, edges;

            if (!(inputParser >> command >> vertices >> edges)) {
                cout << "Invalid command format. Try: Newgraph <vertices> <edges>\n";
                continue;
            }

            // לולאת הזנת קשתות ללא הודעות מיותרות
            for (int i = 0; i < edges; ++i) {
                cout << "Edge " << (i + 1) << ": ";
                if (!getline(cin, inputLine)) {
                    cout << "Error reading edge input. Exiting.\n";
                    close(clientSocket);
                    return 1;
                }
                sendToServer(clientSocket, inputLine + "\n");
                cout << getResponseFromServer(clientSocket) << endl;
            }
        }

        cout << getResponseFromServer(clientSocket) << endl;
    }

    close(clientSocket);
    return 0;
}