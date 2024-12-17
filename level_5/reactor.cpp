#include "reactor.hpp"
#include <iostream>
#include <unistd.h>
#include <vector>

// Start the reactor and return a pointer to it
void* Reactor::startReactor() {
    running = true;
    return this;  // Return this pointer to the current instance
}

int Reactor::addFdToReactor(void *reactorPtr, int fd, ReactorFunc func) {
    Reactor* react = static_cast<Reactor*>(reactorPtr);

    // Check if FD is already added
    for (const auto& pfd : react->pollFds) {
        if (pfd.fd == fd) {
            std::cerr << "FD already added!" << std::endl;  // Error message if FD is already in use
            return -1;  // Return an error code if FD is already added
        }
    }

    // Add the FD to the deque and map the FD to the callback function
    pollfd pfd = {fd, POLLIN, 0};
    react->pollFds.push_back(pfd);
    react->fdToFunc[fd] = func;

    return 0;  // Return success
}

// Remove a file descriptor (fd) from the reactor
int Reactor::removeFdFromReactor(void *reactorPtr, int fd) {
    Reactor* react = static_cast<Reactor*>(reactorPtr);

    // Find and remove FD from deque
    for (auto it = react->pollFds.begin(); it != react->pollFds.end(); ++it) {
        if (it->fd == fd) {
            react->pollFds.erase(it);
            break;
        }
    }

    // Remove callback function associated with the FD
    react->fdToFunc.erase(fd);

    return 0;  // Return success
}

// Stop the reactor
int Reactor::stopReactor(void *reactorPtr) {
    Reactor* react = static_cast<Reactor*>(reactorPtr);
    react->running = false;
    return 0;  // Return success
}

// Run the reactor loop
void Reactor::run() {
    while (running) {
        // Convert deque to vector temporarily to use poll()
        std::vector<pollfd> pollArray(pollFds.begin(), pollFds.end());

        // Poll the file descriptors for events
        int pollResult = poll(pollArray.data(), pollArray.size(), -1);
        if (pollResult < 0) {
            std::cerr << "Error in poll()" << std::endl;
            break;  // Stop loop if error in polling
        }

        // Iterate through all file descriptors to check for readiness
        for (size_t i = 0; i < pollArray.size(); ++i) {
            if (pollArray[i].revents & POLLIN) {  // If there is data available for reading
                // Call the function associated with the FD
                if (fdToFunc.find(pollArray[i].fd) != fdToFunc.end()) {
                    fdToFunc[pollArray[i].fd](pollArray[i].fd);  // Execute the associated callback
                }
            }
        }
    }
}