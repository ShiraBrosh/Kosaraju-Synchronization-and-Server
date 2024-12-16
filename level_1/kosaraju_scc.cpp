#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

/*
how to run:
$ make
$ ./kosaraju_scc

example:
Enter the number of vertices and edges: 5 5
Enter the source and destination: 1 2
Enter the source and destination: 2 3
Enter the source and destination: 3 1
Enter the source and destination: 3 4
Enter the source and destination: 4 5
Strongly Connected Components are:
1 3 2 
4 
5
*/

class Graph
{
    int V; // Number of vertices
    vector<vector<int>> adjacencyList;// Adjacency list

    void DFS(int v, vector<bool> &visited, stack<int> &Stack); 
    void DFSPrint(int v, vector<bool> &visited);

public:
    Graph(int V);
    void addEdge(int v, int w);
    void printSCCs();
    Graph getTransposeGraph();
};

// Constructor
Graph::Graph(int V) 
{
    this->V = V;
    adjacencyList.resize(V); 
}

//add the neighbour to the adjacency list
void Graph::addEdge(int v, int w){
    adjacencyList[v].push_back(w);
}


void Graph::DFS(int v, vector<bool> &visited, stack<int> &Stack)  
{
    visited[v] = true;

    for (int i : adjacencyList[v])
        if (!visited[i])
            DFS(i, visited, Stack);

    Stack.push(v);
}

Graph Graph::getTransposeGraph()
{
    Graph g(V);
    for (int v = 0; v < V; v++)
    {
        for (int i : adjacencyList[v])
            g.adjacencyList[i].push_back(v);
    }
    return g;
}

void Graph::DFSPrint(int v, vector<bool> &visited)
{
    visited[v] = true;
    cout << v + 1 << " "; 

    for (int i : adjacencyList[v])
        if (!visited[i])
            DFSPrint(i, visited);
}

void Graph::printSCCs()
{
    stack<int> Stack;

    vector<bool> visited(V, false);

    for (int i = 0; i < V; i++)
        if (visited[i] == false)
            DFS(i, visited, Stack);

    Graph gr = getTransposeGraph();

    fill(visited.begin(), visited.end(), false); 

    while (!Stack.empty())
    {
        int v = Stack.top();
        Stack.pop();

        if (!visited[v])
        {
            gr.DFSPrint(v, visited);
            cout << endl;
        }
    }
}

int main()
{
    int vertices, edges;

    while (true)
    {
        cout << "Enter the number of vertices and edges: ";
        string vertices_input, edges_input;
        cin >> vertices_input >> edges_input;

        try
        {
            vertices = stoi(vertices_input);
            edges = stoi(edges_input);

            if (vertices > 0 && edges >= 0 && edges <= (vertices - 1) * vertices)
            {
                break; // Valid input, exit the loop
            }
            else
            {
                cout << "Error: Invalid number of vertices or edges. Please try again." << endl;
            }
        }
        catch (invalid_argument &e)
        {
            cout << "Error: Invalid input. Please enter positive integers only. Try again." << endl;
        }
        catch (out_of_range &e)
        {
            cout << "Error: Input out of range. Please enter smaller integers. Try again." << endl;
        }
    }

    Graph g(vertices);

    for (int i = 0; i < edges; ++i)
    {
        while (true)
        {
            cout << "Enter the source and destination: ";
            string src_input, dest_input;
            cin >> src_input >> dest_input;

            try
            {
                int src = stoi(src_input);
                int dest = stoi(dest_input);

                if (src > 0 && dest > 0 && src <= vertices && dest <= vertices)
                {
                    g.addEdge(src - 1, dest - 1); // Adjust for 0-based indexing
                    break; // Valid input, exit the loop
                }
                else
                {
                    cout << "Invalid input. Source and destination must be between 1 and " << vertices << ". Try again." << endl;
                }
            }
            catch (invalid_argument &e)
            {
                cout << "Error: Invalid input. Please enter positive integers only. Try again." << endl;
            }
            catch (out_of_range &e)
            {
                cout << "Error: Input out of range. Please enter smaller integers. Try again." << endl;
            }
        }
    }

    cout << "Strongly Connected Components are:\n";
    g.printSCCs();

    return 0;
}




/*
Kosaraju's Algorithm is used to find all Strongly Connected Components (SCCs) in a directed graph.
A directed graph is strongly connected if there is a path between all pairs of vertices.
A Strongly Connected Component (SCC) of a directed graph is a maximal strongly connected subgraph.
The algorithm works in two steps:
1. DFS traversal of the graph and fill the stack with vertices according to their finish times.
2. Create the transpose of the graph.
3. Pop vertices from the stack one by one and do DFS on the transpose graph.
4. The DFS of the transpose graph gives the SCCs of the original graph.
*/

/*
References:
https://en.wikipedia.org/wiki/Kosaraju%27s_algorithm
https://www.geeksforgeeks.org/strongly-connected-components/
*/