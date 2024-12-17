#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstring>

#define PORT "9034"

using namespace std;

int main(int argc, char *argv[]) {
    int sockfd;
    char buf[256];

    if (argc != 2) {
        cerr << "Usage: client hostname" << endl;
        return 1;
    }

    struct sockaddr_in server_addr;
    struct hostent *server;

    server = gethostbyname(argv[1]);
    if (server == NULL) {
        cerr << "Error: no such host" << endl;
        return 1;
    }

    memset(&server_addr, 0, sizeof server_addr);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(PORT));
    memcpy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1) {
        perror("socket");
        return 1;
    }

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof server_addr) == -1) {
        perror("connect");
        close(sockfd);
        return 1;
    }
    
    while (true) {
        string command;
        cout << "Enter command: ";
        getline(cin, command);

        if (command == "exit") {
            break;
        }

        if (send(sockfd, command.c_str(), command.size(), 0) == -1) {
            perror("send");
            close(sockfd);
            return 1;
        }

        int numbytes = recv(sockfd, buf, sizeof buf - 1, 0);
        if (numbytes == -1) {
            perror("recv");
            close(sockfd);
            return 1;
        }

        buf[numbytes] = '\0';
        cout << "Received: " << buf << endl;
    }

    close(sockfd);

    return 0;
}
