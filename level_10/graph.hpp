#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <deque>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

class Graph {
    int V;                   // Number of vertices
    int maxSCCSize;           // largest strongly connected component size
    int maxEdges;            // Maximum allowed number of edges
    int currentEdges;        // Current number of edges
    vector<deque<int>> adj;  // Adjacency list using deque

    void fillOrder(int v, vector<bool> &visited, deque<int> &Deque); // Helper function for Kosaraju's algorithm
    void DFSUtil(int v, vector<bool> &visited, stringstream &answer , vector<int>& currentSCC); // Depth-first search utility for Kosaraju

public:
    Graph(int V); // Constructor
    bool addEdge(int v, int w); // Adds an edge to the graph
    void removeEdge(int v, int w); // Removes an edge from the graph
    string printSCCs(); // Prints strongly connected components
    Graph getTranspose(); // Returns the transpose of the graph
    int getMaxSCCSize(); // Returns the size of the largest strongly connected component
    int getVertexCount();
};

#endif // GRAPH_HPP
