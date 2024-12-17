#ifndef PROACTOR_HPP
#define PROACTOR_HPP

#include <pthread.h>
#include <map>
#include <functional>
#include <mutex>

// Typedef for callback function
typedef std::function<void(int)> proactorFunc;

class Proactor {
public:
    Proactor();
    ~Proactor();

    // Start a new thread to handle a socket and callback function
    pthread_t startProactor(int sockfd, proactorFunc threadFunc);

    // Stop a thread by thread ID
    int stopProactor(pthread_t tid);

private:
    std::map<pthread_t, std::pair<int, proactorFunc>> proactorMap; // Thread ID -> (Socket, Callback)
    std::mutex mapMutex; // Mutex to protect map access
    static void* threadStart(void* arg); // Entry function for threads

    struct ThreadArgs {
        int sockfd;
        proactorFunc threadFunc;
    };
};

#endif // PROACTOR_HPP