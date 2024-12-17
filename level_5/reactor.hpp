#ifndef REACTOR_HPP
#define REACTOR_HPP

#include <poll.h>
#include <unordered_map>
#include <functional>
#include <deque>

class Reactor {

    using ReactorFunc = std::function<void(int)>;

private:
    std::deque<pollfd> pollFds;               // Using deque for efficient data structure
    std::unordered_map<int, ReactorFunc> fdToFunc; // Map of file descriptors to functions
    bool running;                              // Indicates if the reactor is currently running

public:
    Reactor();
    ~Reactor();

    void *startReactor(); // Start a new reactor and return a pointer to it
    int addFdToReactor(void *reactor, int fd, ReactorFunc func); // Add fd to Reactor and return 0 if successful
    int removeFdFromReactor(void *reactor, int fd); // Remove fd from reactor
    int stopReactor(void *reactor); // Stop reactor
    void run(); // Run the reactor loop
};

#endif