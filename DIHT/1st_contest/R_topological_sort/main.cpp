#include <iostream>
#include <vector>
#include <algorithm>

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
    for (int i = 1; i <= E; ++i) {
        int from_node, to_node;
        cin >> from_node >> to_node;
        nodes[from_node].push_back(to_node);
    }
}

int dfs_visit(vector<vector<int> > &nodes, int node_number, vector<int> &colors, vector<int> &sorted_nodes) {
    colors[node_number] = -1;
    for (int i:nodes[node_number]) {
        if (colors[i] == -1)
            return -1;
        if (colors[i] == 0) {
            if (dfs_visit(nodes, i, colors, sorted_nodes) < 0)
                return -1;
        }
    }
    colors[node_number] = 1;
    sorted_nodes.push_back(node_number);
    return 0;
}

int dfs(Graph &g, vector<int> &sorted_nodes) {
    vector<int> colors(g.V);
    for (int i = 1; i < g.V; ++i) {
        if (colors[i] == 0)
            if(dfs_visit(g.nodes, i, colors, sorted_nodes)<0)
                return -1;
    }
    return 0;
}


int main() {
    int V, E;
    cin >> V >> E;
    Graph g(V, E);
    g.load_graph();

    vector<int> ans;

    if (dfs(g, ans) < 0) {
        cout << -1;
        return 0;
    }

    reverse(ans.begin(),ans.end());

    for (int i: ans)
        cout << i << ' ';

    return 0;
}