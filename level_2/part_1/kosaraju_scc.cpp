#include <iostream>
#include <vector>
#include <stack>
#include <list>
#include <deque>
#include <algorithm>
#include <chrono>

using namespace std;

// choose one of the following to use list or deque for adjacency list
#ifdef USE_LIST
using AdjList = list<int>;
#else
using AdjList = deque<int>;
#endif

class Graph {
    int V; // Number of vertices
    vector<AdjList> adjacencyList; // Adjacency list

    void DFS(int v, vector<bool> &visited, stack<int> &Stack);
    void DFSPrint(int v, vector<bool> &visited);

public:
    Graph(int V);
    void addEdge(int v, int w);
    void printSCCs();
    Graph getTransposeGraph();
};

Graph::Graph(int V) : V(V) {
    adjacencyList.resize(V);
}

void Graph::addEdge(int v, int w) {
    // Check if the edge already exists
    if (find(adjacencyList[v].begin(), adjacencyList[v].end(), w) != adjacencyList[v].end()) {
        cout << "Edge from " << v + 1 << " to " << w + 1 << " already exists.\n";
        return;
    }
    adjacencyList[v].push_back(w);
}

void Graph::DFS(int v, vector<bool> &visited, stack<int> &Stack) {
    visited[v] = true;

    for (int i : adjacencyList[v]) {
        if (!visited[i])
            DFS(i, visited, Stack);
    }
    Stack.push(v);
}

Graph Graph::getTransposeGraph() {
    Graph g(V);
    for (int v = 0; v < V; v++) {
        for (int i : adjacencyList[v])
            g.adjacencyList[i].push_back(v);
    }
    return g;
}

void Graph::DFSPrint(int v, vector<bool> &visited) {
    visited[v] = true;
    cout << v + 1 << " "; // Adjust for 1-based output

    for (int i : adjacencyList[v]) {
        if (!visited[i])
            DFSPrint(i, visited);
    }
}

void Graph::printSCCs() {
    stack<int> Stack;

    vector<bool> visited(V, false);

    for (int i = 0; i < V; i++) {
        if (visited[i] == false)
            DFS(i, visited, Stack);
    }

    Graph gr = getTransposeGraph();

    fill(visited.begin(), visited.end(), false);

    while (!Stack.empty()) {
        int v = Stack.top();
        Stack.pop();

        if (!visited[v]) {
            gr.DFSPrint(v, visited);
            cout << endl;
        }
    }
}

int main() {
    int vertices, edges;
    while (true) {
        cout << "Enter the number of vertices and edges: ";
        if (!(cin >> vertices >> edges)) {
            cerr << "Error: Invalid input for vertices and edges.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (vertices <= 0 || edges < 0 || edges > (vertices - 1) * vertices) {
            cerr << "Error: Invalid number of vertices or edges.\n";
            continue;
        }
        break;
    }

    Graph g(vertices);
    for (int i = 0; i < edges; ++i) {
        int src, dest;
        cout << "Enter the source and destination: ";
        cin >> src >> dest;
        while (cin.fail() || src <= 0 || dest <= 0 || src > vertices || dest > vertices) {
            cout << "Invalid input. Please enter values between 1 and " << vertices << ". Try again." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Enter the source and destination: ";
            cin >> src >> dest;
        }
        g.addEdge(src - 1, dest - 1); // Adjust for 0-based indexing
    }

    auto start = chrono::high_resolution_clock::now();

    cout << "Strongly Connected Components are:\n";
    g.printSCCs();

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "Elapsed Time: " << elapsed.count() << " seconds" << endl;

    return 0;
}
