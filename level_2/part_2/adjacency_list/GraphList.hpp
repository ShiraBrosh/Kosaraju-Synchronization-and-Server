// GraphList.hpp - Class to represent a graph using an adjacency list
#ifndef GRAPHLIST_HPP
#define GRAPHLIST_HPP

#include <vector>
#include <list>
#include <iostream>

class GraphList {
private:
    int V; // Number of vertices
    std::vector<std::list<int>> adjacencyList; // Adjacency list

    void DFS(int v, std::vector<bool>& visited);

public:
    GraphList(int V);
    void addEdge(int v, int w);
    void printSCCs();
};

GraphList::GraphList(int V) : V(V) {
    adjacencyList.resize(V);
}

void GraphList::addEdge(int v, int w) {
    adjacencyList[v].push_back(w);
}

void GraphList::DFS(int v, std::vector<bool>& visited) {
    visited[v] = true;
    std::cout << v + 1 << " ";
    for (int neighbor : adjacencyList[v]) {
        if (!visited[neighbor]) {
            DFS(neighbor, visited);
        }
    }
}

void GraphList::printSCCs() {
    std::vector<bool> visited(V, false);

    for (int i = 0; i < V; ++i) {
        if (!visited[i]) {
            DFS(i, visited);
            std::cout << std::endl;
        }
    }
}

#endif // GRAPHLIST_HPP
