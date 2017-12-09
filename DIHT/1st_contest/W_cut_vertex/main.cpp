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

void dfs_visit(vector<vector<int>> &nodes, int v, int &time,
               vector<bool> &colors,
               vector<bool> &cut_vertexes,
               vector<int> &in_times, vector<int> &up_arr, int &counter, int parent = -1) {
    ++time;
    colors[v] = true;
    in_times[v] = time;
    up_arr[v] = in_times[v]; // init value;
    int children = 0;

    for (auto node: nodes[v]) {
        if (node == parent)
            continue;
        if (colors[node]) {
            // back edge case
            up_arr[v] = min(in_times[node], up_arr[v]);
        } else {
            dfs_visit(nodes, node, time, colors, cut_vertexes, in_times, up_arr, counter, v);
            up_arr[v] = min(up_arr[node], up_arr[v]);

            // if even for one node this is true condition,
            // v -- is a cut_vertex;
            if ((parent != -1) && (in_times[v] <= up_arr[node])) {
                cut_vertexes[v] = true;
                ++counter;
            }
            ++children;
        }
    }
    if ((parent == -1) && (children > 1)) {
        cut_vertexes[v] = true;
        ++counter;
    }
}

int dfs(Graph &g, vector<bool> &cut_vertexes) {

    vector<bool> colors(g.V);
    vector<int> in_times(g.V);
    vector<int> up_arr(g.V);
    int counter = 0;
    int time = 0;

    for (int i = 1; i < g.V; ++i) {
        if (!colors[i]) {
            dfs_visit(g.nodes, i, time, colors, cut_vertexes, in_times, up_arr, counter);
        }
    }
    return counter;
}

int main() {
    int V, E;
    cin >> V>>E;
    Graph g(V, E);
    g.load_graph();

    vector<bool> cut_vertexes(g.V);
    int count = dfs(g, cut_vertexes);
    if (count>0){
        cout<<count<<'\n';
        for(int i=1; i<=g.V; ++i){
            if(cut_vertexes[i])
                cout<<i<<'\n';
        }
    }
    else
        cout<<count;
    return 0;
}