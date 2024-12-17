// reactor.cpp
#include "reactor.hpp"
#include <iostream>
#include <unistd.h>
#include <algorithm>

Reactor::Reactor() : running(false) {}

Reactor::~Reactor() {
    stop();
}

void Reactor::start() {
    running = true;
}

void Reactor::stop() {
    running = false;
}

int Reactor::addFd(int fd, ReactorFunc func) {
    // Add the file descriptor and its callback function to the map
    fdCallbacks[fd] = func;
    struct pollfd pfd;
    pfd.fd = fd;
    pfd.events = POLLIN;
    pollArray.push_back(pfd);
    return 0;
}

int Reactor::removeFd(int fd) {
    // Remove the file descriptor from the map and poll array
    fdCallbacks.erase(fd);
    auto it = std::remove_if(pollArray.begin(), pollArray.end(), [fd](const struct pollfd& pfd) {
        return pfd.fd == fd;
    });
    pollArray.erase(it, pollArray.end());
    return 0;
}

void Reactor::run() {
    while (running) {
        int pollResult = poll(pollArray.data(), pollArray.size(), -1);
        if (pollResult < 0) {
            std::cerr << "Error in poll()" << std::endl;
            break;  // Stop loop if error in polling
        }
        
        for (auto& pfd : pollArray) {
            if (pfd.revents & POLLIN) {
                auto it = fdCallbacks.find(pfd.fd);
                if (it != fdCallbacks.end()) {
                    it->second(pfd.fd);  // Call the associated callback function
                }
            }
        }
    }
}
