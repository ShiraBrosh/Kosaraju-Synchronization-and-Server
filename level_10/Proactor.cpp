#include "Proactor.hpp"
#include <iostream>
#include <unistd.h>

Proactor::Proactor() {}

Proactor::~Proactor() {
    std::lock_guard<std::mutex> lock(mapMutex);
    for (auto& [tid, pair] : proactorMap) {
        close(pair.first); // Close socket
        pthread_cancel(tid); // Cancel thread
    }
}

pthread_t Proactor::startProactor(int sockfd, proactorFunc threadFunc) {
    std::lock_guard<std::mutex> lock(mapMutex);

    ThreadArgs* args = new ThreadArgs{sockfd, threadFunc};
    pthread_t tid;

    if (pthread_create(&tid, nullptr, threadStart, args) != 0) {
        std::cerr << "Failed to create thread\n";
        delete args;
        return 0;
    }

    proactorMap[tid] = {sockfd, threadFunc};
    return tid;
}

int Proactor::stopProactor(pthread_t tid) {
    std::lock_guard<std::mutex> lock(mapMutex);

    auto it = proactorMap.find(tid);
    if (it == proactorMap.end()) return -1; // Thread not found

    close(it->second.first); // Close socket
    pthread_cancel(tid); // Cancel thread
    proactorMap.erase(it); // Remove from map
    return 0;
}

void* Proactor::threadStart(void* arg) {
    ThreadArgs* args = static_cast<ThreadArgs*>(arg);
    args->threadFunc(args->sockfd);
    delete args;
    return nullptr;
}