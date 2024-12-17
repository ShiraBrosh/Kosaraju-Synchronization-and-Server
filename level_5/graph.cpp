#include "graph.hpp"

Graph::Graph(int V) {
    this->V = V;
    adj.resize(V);
}

bool Graph::addEdge(int v, int w) {
    if (find(adj[v].begin(), adj[v].end(), w) != adj[v].end()) {
        return false;
    }
    adj[v].push_back(w);
    return true;
}

void Graph::removeEdge(int v, int w) {
    adj[v].erase(remove(adj[v].begin(), adj[v].end(), w), adj[v].end());
}

void Graph::fillOrder(int v, vector<bool> &visited, deque<int> &Deque) {
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

void Graph::DFSUtil(int v, vector<bool> &visited, stringstream &answer) {
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

    for (int i = 0; i < V; i++)
        if (!visited[i])
            fillOrder(i, visited, Deque);

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

// Implementation of getCommand function
Command getCommand(const string &command) {
    if (command == "Newgraph") return Newgraph;
    if (command == "Kosaraju") return Kosaraju;
    if (command == "Newedge") return Newedge;
    if (command == "Removeedge") return Removeedge;
    if (command == "Exit") return Exit;
    if (command == "") return Blank;

    return Invalid;
}