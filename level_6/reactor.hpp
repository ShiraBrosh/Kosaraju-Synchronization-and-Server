// reactor.hpp
#ifndef REACTOR_HPP
#define REACTOR_HPP

#include <functional>
#include <unordered_map>
#include <poll.h>
#include <vector>

// Define a function pointer type for callback functions
typedef std::function<void(int)> ReactorFunc;

class Reactor {
public:
    Reactor();
    ~Reactor();

    void start();
    void stop();
    int addFd(int fd, ReactorFunc func);
    int removeFd(int fd);
    void run();

private:
    std::unordered_map<int, ReactorFunc> fdCallbacks;  // Map of file descriptors to their respective callback functions
    std::vector<struct pollfd> pollArray;  // Array of file descriptors to poll
    bool running;
};

#endif // REACTOR_HPP
