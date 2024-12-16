#include <iostream>
#include <vector>
#include <deque>
#include <string>
#include <sstream>
#include <algorithm>
#include <limits>

/*
how to run
# cd level_3
$ make
$ ./kosaraju_deque

output:
Please insert one of the following commands:

Newgraph <n> <m> - Create a new graph with n vertices and m edges
Kosaraju - Print SCCs of the graph
Newedge <i> <j> - Add edge from vertex i to vertex j
Removeedge <i> <j> - Remove edge from vertex i to vertex j
Exit - to exit 

*/
using namespace std;

class Graph
{
    int V;                   // Number of vertices
    vector<deque<int>> adj;  // Adjacency list

    void fillOrder(int v, vector<bool> &visited, deque<int> &Deque);
    void DFSUtil(int v, vector<bool> &visited);

public:
    Graph(int V);
    bool addEdge(int v, int w);
    void removeEdge(int v, int w);
    void printSCCs();
    Graph getTranspose();
};

Graph::Graph(int V)
{
    this->V = V;
    adj.resize(V);
}

bool Graph::addEdge(int v, int w)
{
    if (find(adj[v].begin(), adj[v].end(), w) != adj[v].end()) {return false;}
    adj[v].push_back(w);
    return true;
}

void Graph::removeEdge(int v, int w)
{
    adj[v].erase(remove(adj[v].begin(), adj[v].end(), w), adj[v].end());
}

void Graph::fillOrder(int v, vector<bool> &visited, deque<int> &Deque)
{
    visited[v] = true;
    for (int i : adj[v])
        if (!visited[i])
            fillOrder(i, visited, Deque);
    Deque.push_back(v);
}

Graph Graph::getTranspose()
{
    Graph g(V);
    for (int v = 0; v < V; v++)
    {
        for (int i : adj[v])
            g.adj[i].push_back(v);
    }
    return g;
}

void Graph::DFSUtil(int v, vector<bool> &visited)
{
    visited[v] = true;
    cout << v + 1 << " ";
    for (int i : adj[v])
        if (!visited[i])
            DFSUtil(i, visited);
}

void Graph::printSCCs()
{
    deque<int> Deque;
    vector<bool> visited(V, false);
    for (int i = 0; i < V; i++)
        if (!visited[i])
            fillOrder(i, visited, Deque);

    Graph gr = getTranspose();
    fill(visited.begin(), visited.end(), false);

    int componentNumber = 1;
    while (!Deque.empty())
    {
        int v = Deque.back();
        Deque.pop_back();
        if (!visited[v])
        {
            cout << "Strongly Connected Component " << componentNumber++ << ":\n";
            gr.DFSUtil(v, visited);
            cout << endl;
        }
    }
}

enum Command {
    Newgraph,
    Kosaraju,
    Newedge,
    Removeedge,
    Exit,
    Blank,
    Invalid
};

Command getCommand(const string &command) {
    if (command == "Newgraph") return Newgraph;
    if (command == "Kosaraju") return Kosaraju;
    if (command == "Newedge") return Newedge;
    if (command == "Removeedge") return Removeedge;
    if (command == "Exit") return Exit;
    if (command == "") return Blank;
    
    return Invalid;
}

int main()
{
    Graph *g = nullptr;
    string line;

    cout << "Please insert one of the following commands:\n" << endl;
    cout << "Newgraph <n> <m> - Create a new graph with n vertices and m edges" << endl;
    cout << "Kosaraju - Print SCCs of the graph" << endl;
    cout << "Newedge <i> <j> - Add edge from vertex i to vertex j" << endl;
    cout << "Removeedge <i> <j> - Remove edge from vertex i to vertex j" << endl;
    cout << "Exit - to exit \n" << endl;

    while (getline(cin, line)) {
        istringstream iss(line);
        string commandStr;
        iss >> commandStr;
        
        Command command = getCommand(commandStr);

        switch (command) {
            case Newgraph: {
                int vertices, edges;
                if (iss >> vertices >> edges && vertices > 0 && edges >= 0) {
                    g = new Graph(vertices);
                    cout << "Created new graph with " << vertices << " vertices and " << edges << " edges" << endl;
                    
                    for (int i = 0; i < edges; ++i) {
                        int src, dest;
                        cout << "Enter the source and destination: ";
                        if (cin >> src >> dest && src > 0 && dest > 0 && src <= vertices && dest <= vertices) {
                            g->addEdge(src - 1, dest - 1);
                            cout << "Added edge from " << src << " to " << dest << endl;
                        } else {
                            cout << "Invalid edge input. Please provide valid vertex numbers between 1 and " << vertices << endl;
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        }
                    }
                } else {
                    cout << "Invalid graph size. Please enter positive values for vertices and edges." << endl;
                }
                break;
            }
            case Kosaraju:
                if (g) {
                    cout << "Running Kosaraju's algorithm..." << endl;
                    g->printSCCs();
                } else {
                    cout << "No graph created yet." << endl;
                }
                break;
            case Newedge: {
                int i, j;
                if (iss >> i >> j && i > 0 && j > 0) {
                    if (g) {
                        if (g->addEdge(i - 1, j - 1)) {
                            cout << "Added edge from " << i << " to " << j << endl;
                        } else {
                            cout << "Edge already exists." << endl;
                        }
                    } else {
                        cout << "No graph created yet." << endl;
                    }
                } else {
                    cout << "Invalid edge input. Please provide valid vertex numbers." << endl;
                }
                break;
            }
            case Removeedge: {
                int i, j;
                if (iss >> i >> j && i > 0 && j > 0) {
                    if (g) {
                        g->removeEdge(i - 1, j - 1);
                        cout << "Removed edge from " << i << " to " << j << endl;
                    } else {
                        cout << "No graph created yet." << endl;
                    }
                } else {
                    cout << "Invalid edge input. Please provide valid vertex numbers." << endl;
                }
                break;
            }
            case Exit:
                delete g;
                return 0;
            case Blank:
                break;
            default:
                cout << "Invalid command" << endl;
                break;
        }
    }

    delete g;
    return 0;
}