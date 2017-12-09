#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Graph {
public:
    int V, E;

    Graph(const Graph &g) = delete;

    // v+1 because of count starts with 1st node
    Graph(int _v, int _e) : V(_v + 1), E(_e), nodes(V, vector<int>()) {};

    void load_graph();

    vector<vector<int> > nodes;
};

void Graph::load_graph() {
    int from_node, to_node;
    for (int i = 1; i <= E; ++i) {
        cin >> from_node >> to_node;
        nodes[from_node].push_back(to_node);
        nodes[to_node].push_back(from_node);
    }
}


int main() {
    int V, E;
    cin >> V >> E;
    Graph g(V, E);
    g.load_graph();
    int count = 0;
    for (auto v: g.nodes)
        if(v.size()&1)
            ++count;

    cout << count/2;
    return 0;
}