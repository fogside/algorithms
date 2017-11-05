#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

class Graph {
public:
    int V, E;

    Graph(const Graph &g) = delete;

    Graph(int _v, int _e) : V(_v + 1), E(_e), nodes(V, vector<int>()) {};

    void load_graph();

    void print_graph();

    vector<vector<int> > nodes;
};

void Graph::load_graph() {
    int from_node, to_node;
    for (int i = 1; i <= E; ++i) {
        cin >> from_node >> to_node;
        nodes[from_node].push_back(to_node);
    }
}

void Graph::print_graph() {
    for (int i = 1; i < V; ++i) {
        cout << "Vertex: " << i << " nodes: ";
        for (auto j: nodes[i])
            cout << j << ' ';
        cout << '\n';
    }
}

int dfs_visit(vector<vector<int> > &nodes, vector<int> &colors, int v, vector<int> &path) {
    colors[v] = -1;
    for (int i:nodes[v]) {
        if (!colors[i]) {
            path[i] = v;
            colors[i] = true;
            int dfs_res = dfs_visit(nodes, colors, i, path);
            if (dfs_res != 0)
                return dfs_res;
        } else if (colors[i]==-1) {
            path.push_back(v);
            return i;
        }
    }
    colors[v] = 1;
    return 0;
}

int dfs(Graph &g, vector<int> &ans) {
    vector<int> path(g.V);
    vector<int> colors(g.V);
    for (int i = 1; i < g.V; ++i) {
        if (!colors[i]) {
            int dfs_res = dfs_visit(g.nodes, colors, i, path);
            if (dfs_res != 0) {
                for (int tmp = path.back(); tmp != dfs_res; tmp = path[tmp]) {
                    ans.push_back(tmp);
                }
                ans.push_back(dfs_res);
                return dfs_res;
            }
        }
    }
    return 0;
}

int main() {

    int V, E;
    cin >> V >> E;
    Graph g(V, E);
    g.load_graph();

    vector<int> ans;
    int loop_len = dfs(g, ans);
    if (loop_len > 0) {
        cout << "YES" << '\n';
        for (auto i = ans.rbegin(); i != ans.rend(); ++i)
            cout << *i << ' ';
    } else
        cout << "NO";
    return 0;
}