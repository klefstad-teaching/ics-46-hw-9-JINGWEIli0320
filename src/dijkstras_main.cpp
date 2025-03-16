#include "dijkstras.h"
#include <iostream>
#include <stdexcept>
using namespace std;

int main() {
    string filename;
    cout << "Enter the graph file name: ";
    cin >> filename;

    Graph G;
    try {
        file_to_graph(filename, G);
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    int source, destination;
    cout << "Enter source vertex: ";
    cin >> source;
    cout << "Enter destination vertex: ";
    cin >> destination;

    vector<int> previous;
    vector<int> distances = dijkstra_shortest_path(G, source, previous);

    if (distances[destination] == INF) {
        cout << "No path from " << source << " to " << destination << " exists." << endl;
    } else {
        vector<int> path = extract_shortest_path(distances, previous, destination);
        print_path(path, distances[destination]);
    }

    return 0;
}
