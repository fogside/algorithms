#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <tuple>

using namespace std;

class Graph {
public:
    int V, E;

    Graph(const Graph &g) = delete;
    Graph(int _v, int _e) : V(_v + 1), E(_e), straight_nodes(V, vector<int>()), reverse_nodes(V, vector<int>()) {};
    void load_graph();
    void print_graph();
    vector<vector<int> > straight_nodes;
    vector<vector<int> > reverse_nodes;
};

void Graph::load_graph() {
    for (int i = 1; i <= E; ++i) {
        int from_node, to_node;
        cin >> from_node >> to_node;
        straight_nodes[from_node].push_back(to_node);
        reverse_nodes[to_node].push_back(from_node);
    }
}

void Graph::print_graph() {
    for (int i = 1; i < V; ++i) {
        cout << "Vertex: " << i << " nodes: ";
        for (auto j: straight_nodes[i])
            cout << j << ' ';
        cout << '\n';
    }
}

void dfs_visit(vector<vector<int> > &nodes, int vertex_number, int &time, vector<bool> &used, vector<int> &exit_time) {

    time++;
    used[vertex_number] = true;

    for (auto j: nodes[vertex_number])
        if (!used[j]) {
            used[j] = true;
            dfs_visit(nodes, j, time, used, exit_time);
        }

    time++;
    exit_time[vertex_number] = time;

}

vector<int> dfs(Graph &g) {
    /* return exit time
     * for each vertex in graph g for straight path */

    int time = 0;
    vector<int> exit_time(g.V);
    vector<bool> used(g.V);

    for (int i = 1; i < g.V; ++i) {
        if (!used[i]) {
            dfs_visit(g.straight_nodes, i, time, used, exit_time);
        }
    }
    return exit_time;
}


tuple<int, vector<int> > topological_sort(Graph &g, vector<int> &exit_times) {

    vector<int> components(g.V);

    // sort nodes according to exit_time in descending order
    vector<int> sorted_nodes(exit_times.size());
    size_t n(0);
    generate(std::begin(sorted_nodes), std::end(sorted_nodes), [&] { return n++; });
    sort(sorted_nodes.begin(), sorted_nodes.end(),
         [&](int i1, int i2) { return exit_times[i1] > exit_times[i2]; });


    // start dfs with first
    int time = 0;
    int components_counter = 0;
    vector<bool> used(g.V);
    vector<int> exit_times_tmp(g.V);

    for (auto i = sorted_nodes.begin(); i!=sorted_nodes.end()-1; ++i) {
        if (!used[*i]) {
            dfs_visit(g.reverse_nodes, *i, time, used, exit_times_tmp);
            components_counter++;
            for (int k = 1; k < g.V; ++k) {
                if (used[k] && (components[k] == 0))
                    components[k] = components_counter;
            }
        }
    }
    return make_tuple(components_counter, components);
}

int main() {
    int V, E;
    cin >> V >> E;
    Graph g(V, E);
    g.load_graph();
    vector<int> exit_times = dfs(g);

    vector<int> result_markup;
    int components_number;
    tie(components_number, result_markup) = topological_sort(g, exit_times);
    cout<<components_number<<'\n';
    for (int i = 1; i < g.V; ++i)
        cout << result_markup[i] << ' ';

    return 0;
}