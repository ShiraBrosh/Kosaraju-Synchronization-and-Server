#include "graph.hpp"
#include <iostream> // Required for std::cout

Graph::Graph(int V) : V(V), currentEdges(0) {
    maxEdges = V * (V - 1); // Maximum number of edges in a directed graph without self-loops
    adj.resize(V);
}

bool Graph::addEdge(int v, int w) {
    // Validate vertices
    if (v < 0 || v >= V || w < 0 || w >= V) {
        std::cout << "Error: Vertex out of range." << std::endl;
        return false;
    }

    // Check if maximum number of edges is reached
    if (currentEdges >= maxEdges) {
        std::cout << "Error: Maximum number of edges reached." << std::endl;
        return false;
    }

    // Check if the edge already exists
    if (find(adj[v].begin(), adj[v].end(), w) != adj[v].end()) {
        std::cout << "Error: Edge already exists between vertices " << v + 1 << " and " << w + 1 << "." << std::endl;
        return false;
    }

    // Add the edge
    adj[v].push_back(w);
    currentEdges++;

    // Notify if graph is now full
    if (currentEdges >= maxEdges) {
        std::cout << "Graph has reached the maximum number of edges." << std::endl;
    }

    return true;
}

void Graph::removeEdge(int v, int w) {
    // Validate vertices
    if (v < 0 || v >= V || w < 0 || w >= V) {
        std::cout << "Error: Vertex out of range." << std::endl;
        return;
    }

    // Remove the edge if it exists
    auto& edges = adj[v];
    auto it = std::find(edges.begin(), edges.end(), w);
    if (it != edges.end()) {
        edges.erase(it);
        currentEdges--;
    }
}

void Graph::fillOrder(int v, vector<bool>& visited, deque<int>& Deque) {
    visited[v] = true;
    for (int i : adj[v])
        if (!visited[i])
            fillOrder(i, visited, Deque);
    Deque.push_back(v);
}

Graph Graph::getTranspose() {
    Graph g(V);
    for (int v = 0; v < V; v++) {
        for (int i : adj[v])
            g.adj[i].push_back(v);
    }
    return g;
}

void Graph::DFSUtil(int v, vector<bool>& visited, stringstream& answer) {
    visited[v] = true;
    answer << v + 1 << " ";
    for (int i : adj[v])
        if (!visited[i])
            DFSUtil(i, visited, answer);
}

string Graph::printSCCs() {
    deque<int> Deque;
    vector<bool> visited(V, false);
    stringstream answer;

    // Step 1: Fill vertices in stack according to their finishing times
    for (int i = 0; i < V; i++) {
        if (!visited[i])
            fillOrder(i, visited, Deque);
    }

    // Step 2: Create a transpose of the graph
    Graph gr = getTranspose();

    // Step 3: Reset visited array for second DFS
    fill(visited.begin(), visited.end(), false);

    // Step 4: Process all vertices in order defined by stack
    int componentCount = 0;
    while (!Deque.empty()) {
        int v = Deque.back();
        Deque.pop_back();

        if (!visited[v]) {
            answer << "Strongly Connected Component " << ++componentCount << ":\n";
            gr.DFSUtil(v, visited, answer);
            answer << "\n";
        }
    }

    return answer.str();
}
