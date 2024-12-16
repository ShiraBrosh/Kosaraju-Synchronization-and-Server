
// main_matrix.cpp
#include <iostream>
#include <vector>
#include <chrono>
#include "GraphMatrix.hpp"

using namespace std;

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

    GraphMatrix g(vertices);
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