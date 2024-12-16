// GraphMatrix.hpp - Class to represent a graph using an adjacency matrix
#ifndef GRAPHMATRIX_HPP
#define GRAPHMATRIX_HPP

#include <vector>
#include <iostream>

class GraphMatrix {
private:
    int V; // Number of vertices
    std::vector<std::vector<bool>> adjacencyMatrix; // Adjacency matrix

    void DFS(int v, std::vector<bool>& visited);

public:
    GraphMatrix(int V);
    void addEdge(int v, int w);
    void printSCCs();
};

GraphMatrix::GraphMatrix(int V) : V(V) {
    adjacencyMatrix.resize(V, std::vector<bool>(V, false));
}

void GraphMatrix::addEdge(int v, int w) {
    adjacencyMatrix[v][w] = true;
}

void GraphMatrix::DFS(int v, std::vector<bool>& visited) {
    visited[v] = true;
    std::cout << v + 1 << " ";
    for (int i = 0; i < V; ++i) {
        if (adjacencyMatrix[v][i] && !visited[i]) {
            DFS(i, visited);
        }
    }
}

void GraphMatrix::printSCCs() {
    std::vector<bool> visited(V, false);

    for (int i = 0; i < V; ++i) {
        if (!visited[i]) {
            DFS(i, visited);
            std::cout << std::endl;
        }
    }
}

#endif // GRAPHMATRIX_HPP
