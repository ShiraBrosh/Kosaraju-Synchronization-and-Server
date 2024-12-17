#include <iostream>
#include <sys/socket.h>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 9034

using namespace std;

void sendToServer(int socket, const string &msg) {
    string message = msg + "\n"; // Append newline to signal end of command
    if (send(socket, message.c_str(), message.size(), 0) == -1) {
        cerr << "Failed to send message to server.\n";
    } else {
        cout << "Sent to server: " << message;
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

    cout << "Connected to server.\n";

    string inputLine;

    while (true) {
        cout << "> ";  // Prompt for user input
        if (!getline(cin, inputLine)) {
            cout << "Input error, closing client.\n";
            break;
        }

        sendToServer(clientSocket, inputLine);
        cout << getResponseFromServer(clientSocket) << endl;
    }

    close(clientSocket);
    return 0;
}
