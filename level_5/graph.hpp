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
    vector<deque<int>> adj;  // Adjacency list using deque

    void fillOrder(int v, vector<bool> &visited, deque<int> &Deque);
    void DFSUtil(int v, vector<bool> &visited, stringstream &answer);

public:
    Graph(int V);
    bool addEdge(int v, int w);
    void removeEdge(int v, int w);
    string printSCCs();
    Graph getTranspose();
};

// Enumeration for the command types
enum Command {
    Newgraph,
    Kosaraju,
    Newedge,
    Removeedge,
    Exit,
    Blank,
    Invalid
};

// Function to determine the command from a string
Command getCommand(const string &command);

#endif // GRAPH_HPP