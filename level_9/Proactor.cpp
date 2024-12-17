#include "Proactor.hpp"
#include <stdexcept>
#include <unistd.h>

// Thread entry point
void* Proactor::threadStart(void* arg) {
    auto* pair = static_cast<std::pair<int, proactorFunc>*>(arg);
    int sockfd = pair->first;
    proactorFunc func = pair->second;

    func(sockfd); // Execute the callback function

    delete pair; // Clean up dynamically allocated memory
    return nullptr;
}

// Constructor
Proactor::Proactor() {}

// Destructor: Stop all active threads
Proactor::~Proactor() {
    for (auto& [tid, pair] : proactorMap) {
        stopProactor(tid);
    }
}

// Start a new thread for handling a socket
pthread_t Proactor::startProactor(int sockfd, proactorFunc threadFunc) {
    pthread_t tid;

    // Allocate memory for the argument pair
    auto* pair = new std::pair<int, proactorFunc>(sockfd, threadFunc);

    // Create the thread
    if (pthread_create(&tid, nullptr, threadStart, pair) != 0) {
        delete pair; // Cleanup on failure
        close(sockfd); // Close the socket if thread creation fails
        throw std::runtime_error("Failed to create thread");
    }

    proactorMap[tid] = {sockfd, threadFunc}; // Track thread and socket
    return tid;
}

// Stop a thread by thread ID
int Proactor::stopProactor(pthread_t tid) {
    if (proactorMap.find(tid) == proactorMap.end()) {
        return -1; // Thread not found
    }

    auto [sockfd, _] = proactorMap[tid]; // Get the socket associated with the thread

    if (pthread_cancel(tid) != 0) {
        return -1; // Failed to cancel the thread
    }

    if (pthread_join(tid, nullptr) != 0) {
        return -1; // Failed to join the thread
    }

    close(sockfd); // Close the socket
    proactorMap.erase(tid); // Remove from the map

    return 0; // Success
}