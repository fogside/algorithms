#include <iostream>
#include <vector>
#include <queue>

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
    int from_node, to_node;
    for (int i = 1; i <= E; ++i) {
        cin >> from_node >> to_node;
        nodes[from_node].push_back(to_node);
        nodes[to_node].push_back(from_node);
    }
}

int bfs(Graph &g) {
    vector<int> colors(g.V);
    queue<int> Q;

    for (int i = 1; i < g.V; ++i)
        if (!colors[i]) {
            Q.push(i);
            while (!Q.empty()) {
                int x = Q.front();
                Q.pop();
                for (int v:g.nodes[x]) {
                    if (colors[v] && (colors[v] % 2 == colors[x] % 2))
                        return 1;
                    if (!colors[v]) {
                        colors[v] = colors[x] + 1;
                        Q.push(v);
                    }
                }

            }
        }

    return 0;
}


int main() {
    int V, E;
    cin >> V >> E;
    Graph g(V,E);
    g.load_graph();
    if(bfs(g))
        cout<<"NO";
    else
        cout<<"YES";
    return 0;
}