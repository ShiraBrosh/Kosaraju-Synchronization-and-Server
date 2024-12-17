#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <deque>
#include <string>
#include <sstream>
#include <algorithm>
#include <mutex> // For thread safety

using namespace std;

class Graph {
private:
    int V;                   // Number of vertices
    vector<deque<int>> adj;  // Adjacency list using deque
    static Graph* instance;  // Singleton instance
    static mutex mtx;        // Mutex for thread safety

    // Private constructor (to prevent external instantiation)
    Graph(int V = 0);

    // Helper functions
    void fillOrder(int v, vector<bool>& visited, deque<int>& Deque);
    void DFSUtil(int v, vector<bool>& visited, stringstream& answer);

public:
    // Static method to get the singleton instance
    static Graph* getInstance(int V = 0);

    // Function to reset the graph (clear data and resize)
    void resetGraph(int V);

    // Graph operations
    bool addEdge(int v, int w);
    void removeEdge(int v, int w);
    string printSCCs();
    Graph getTranspose();
};

#endif // GRAPH_HPP
