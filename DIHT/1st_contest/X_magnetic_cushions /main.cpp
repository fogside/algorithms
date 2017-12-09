#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>

using namespace std;

class Graph {
public:
    int V, E;

    Graph(const Graph &g) = delete;

    // v+1 because of count starts with 1st node
    Graph(int _v, int _e) : V(_v + 1), E(_e), nodes(V, vector<pair<int, int> >()) {};

    void load_graph();

    vector<vector<pair<int, int> > > nodes;
};

void Graph::load_graph() {
    int node1, node2, node3;
    for (int i = 1; i <= E; ++i) {
        cin >> node1 >> node2 >> node3;

        nodes[node1].emplace_back(make_pair(i, node2));
        nodes[node1].emplace_back(make_pair(i, node3));

        nodes[node2].emplace_back(make_pair(i, node1));
        nodes[node2].emplace_back(make_pair(i, node3));

        nodes[node3].emplace_back(make_pair(i, node1));
        nodes[node3].emplace_back(make_pair(i, node2));
    }
}

void dfs_visit(vector<vector<pair<int, int> > > &nodes, int v, int &time,
               vector<bool> &colors,
               vector<int> &bridges,
               vector<int> &in_times,
               vector<int> &up_arr,
               int parent_edge_num = -1,
               int parent_vertex = -1) {
    ++time;
    colors[v] = true;
    in_times[v] = time;
    up_arr[v] = in_times[v]; // init value;

    for (auto node: nodes[v]) {
        int edge_num = node.first;
        int to = node.second;

        if ((edge_num != parent_edge_num) && (to == parent_vertex)){
            --bridges[parent_edge_num];
            --bridges[edge_num];
            continue;
        }

        if((edge_num == parent_edge_num) && (to ==parent_vertex))
            continue;

        if (colors[to]) {
            // back edge case
            up_arr[v] = min(in_times[to], up_arr[v]);
        } else {
            dfs_visit(nodes, to, time, colors, bridges, in_times, up_arr, edge_num, v);
            up_arr[v] = min(up_arr[to], up_arr[v]);

            if (in_times[v] < up_arr[to]) {
                bridges[edge_num] +=1 ;
            }
        }
    }
}

int dfs(Graph &g, vector<int> &bridges) {

    vector<bool> colors(g.V);
    vector<int> in_times(g.V);
    vector<int> up_arr(g.V);
    int time = 0;

    for (int i = 1; i < g.V; ++i) {
        if (!colors[i]) {
            dfs_visit(g.nodes, i, time, colors, bridges, in_times, up_arr);
        }
    }
    int counter = 0;
    for(int i: bridges)
        if (i>0)
            ++counter;
    return counter;
}




int main() {
    return 0;
}