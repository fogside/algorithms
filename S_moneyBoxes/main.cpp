#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

class Graph {
public:
    int V, E;

    Graph(const Graph &g) = delete;

    Graph(int _v, int _e) : V(_v + 1), E(_e), nodes(V, vector<int>()) {};

    void load_graph();

    vector<vector<int> > nodes;
};

void Graph::load_graph() {
    int from_node;
    for (int i = 1; i <= E; ++i) {
        cin >> from_node;
        nodes[from_node].push_back(i);
    }
}

int dfs_visit(vector<vector<int> > &nodes, int node_number, vector<bool> &colors) {
    colors[node_number] = true;
    for (int i:nodes[node_number]) {
        if (!colors[i]) {
            dfs_visit(nodes, i, colors);
        }
    }
    return 0;
}


int dfs(Graph &g) {

    if(g.V == 2)
        return 1;

    // apply dfs to count nodes connected with current one;
    vector<bool> colors(g.V);
    vector<int> dfs_count(g.V);
    for(int i = 1; i<g.V; ++i){
        dfs_visit(g.nodes,i, colors);
        dfs_count[i] = accumulate(colors.begin(),colors.end(),0);
        fill(colors.begin(), colors.end(), 0);
    }

    vector<int> sorted_nodes(g.V-1);
    size_t n(1);
    generate(std::begin(sorted_nodes), std::end(sorted_nodes), [&] { return n++; });
    sort(sorted_nodes.begin(), sorted_nodes.end(),
         [&](int i1, int i2) { return dfs_count[i1] > dfs_count[i2]; });

    // now apply dfs once again on this sorted nodes;
    int components_counter = 0;
    for (int i:sorted_nodes) {
        if (!colors[i]) {
            dfs_visit(g.nodes, i, colors);
            components_counter++;
        }
    }
    return components_counter;
}


int main() {
    int V;
    cin >> V;
    Graph g(V, V);
    g.load_graph();

    int ans = dfs(g);
    cout << ans;

    return 0;
}