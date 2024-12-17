#include "graph.hpp"

// Static member definitions
Graph* Graph::instance = nullptr;
mutex Graph::mtx;

// Private constructor
Graph::Graph(int V) : V(V) {
    adj.resize(V);
}

// Static function to get the singleton instance
Graph* Graph::getInstance(int V) {
    lock_guard<mutex> lock(mtx); // Ensure thread safety
    if (!instance) {
        instance = new Graph(V);
    }
    return instance;
}

// Reset the graph with new number of vertices
void Graph::resetGraph(int V) {
    lock_guard<mutex> lock(mtx); // Ensure thread safety
    this->V = V;
    adj.clear();
    adj.resize(V);
}

// Add an edge to the graph
bool Graph::addEdge(int v, int w) {
    lock_guard<mutex> lock(mtx); // Ensure thread safety
    if (find(adj[v].begin(), adj[v].end(), w) != adj[v].end()) {
        return false;
    }
    adj[v].push_back(w);
    return true;
}

// Remove an edge from the graph
void Graph::removeEdge(int v, int w) {
    lock_guard<mutex> lock(mtx); // Ensure thread safety
    adj[v].erase(remove(adj[v].begin(), adj[v].end(), w), adj[v].end());
}

// Transpose the graph
Graph Graph::getTranspose() {
    Graph g(V);
    for (int v = 0; v < V; v++) {
        for (int i : adj[v]) {
            g.adj[i].push_back(v);
        }
    }
    return g;
}

// Utility to perform DFS and fill order of vertices
void Graph::fillOrder(int v, vector<bool>& visited, deque<int>& Deque) {
    visited[v] = true;
    for (int i : adj[v]) {
        if (!visited[i]) {
            fillOrder(i, visited, Deque);
        }
    }
    Deque.push_back(v);
}

// Utility for DFS traversal
void Graph::DFSUtil(int v, vector<bool>& visited, stringstream& answer) {
    visited[v] = true;
    answer << v + 1 << " ";
    for (int i : adj[v]) {
        if (!visited[i]) {
            DFSUtil(i, visited, answer);
        }
    }
}

// Find and print Strongly Connected Components
string Graph::printSCCs() {
    deque<int> Deque;
    vector<bool> visited(V, false);
    stringstream answer;

    for (int i = 0; i < V; i++) {
        if (!visited[i]) {
            fillOrder(i, visited, Deque);
        }
    }

    Graph gr = getTranspose();
    fill(visited.begin(), visited.end(), false);

    int componentCount = 0;
    while (!Deque.empty()) {
        int v = Deque.back();
        Deque.pop_back();
        if (!visited[v]) {
            answer << "Strongly Connected Component " << ++componentCount << ":\n";
            gr.DFSUtil(v, visited, answer);
            answer << endl;
        }
    }
    return answer.str();
}
