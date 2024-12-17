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
    return string(buffer);
}

int main() {
    cout << "Starting client..." << endl;

    int clientSocket;
    sockaddr_in serverAddr;

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        perror("Error creating socket");
        return -1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) <= 0) {
        perror("Invalid address");
        close(clientSocket);
        return -1;
    }

    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Connection failed");
        close(clientSocket);
        return -1;
    }

    cout << getResponseFromServer(clientSocket);

    string inputLine;

    while (true) {
        cout << "> ";
        if (!getline(cin, inputLine)) {
            cout << "Error reading input.\n";
            break;
        }

        sendToServer(clientSocket, inputLine + "\n");

        if (inputLine.rfind("Newgraph", 0) == 0) {
            istringstream iss(inputLine);
            string command;
            int vertices, edges;
            if (!(iss >> command >> vertices >> edges)) {
                cout << "Invalid command format. Use: Newgraph <vertices> <edges>\n";
                continue;
            }

            for (int i = 0; i < edges; ++i) {
                cout << "Enter edge " << (i + 1) << ": ";
                if (!getline(cin, inputLine)) {
                    cout << "Error reading edge input.\n";
                    close(clientSocket);
                    return -1;
                }
                sendToServer(clientSocket, inputLine + "\n");
                cout << getResponseFromServer(clientSocket);
            }
        }

        cout << getResponseFromServer(clientSocket);
    }

    close(clientSocket);
    return 0;
}