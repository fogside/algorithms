#include <iostream>
#include <vector>
#include <algorithm>
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

int get_path(int v, vector<int> &edges, vector<int> &ans);

int bfs(Graph &g, vector<int> &ans, int from_node, int to_node) {
    vector<bool> colors(g.V);
    vector<int>edges(g.V);
    vector<int>weights(g.V);

    queue<int> Q;
    Q.push(from_node);
    colors[from_node]=true;

    while ((!Q.empty())) {
        int x = Q.front();
        Q.pop();
        for (auto v:g.nodes[x]) {
            if (v == to_node) {
                edges[v]=x;
                colors[v]= true;
                Q = queue<int>(); // clear the queue to exit main loop
                break;
            }
            if (!colors[v]) {
                colors[v] = true;
                edges[v]=x;
                Q.push(v);
            }
        }
    }

    get_path(to_node, edges, ans);
    reverse(ans.begin(), ans.end());

    if (!colors[to_node])
        return -1;
    else
        return ans.size()-1;
}

int get_path(int v, vector<int> &edges, vector<int> &ans){
    ans.push_back(v);
    for (;edges[v]!=0;v=edges[v]){
        ans.push_back(edges[v]);
    }
}


int main() {
    int V, E, from_node, to_node;
    cin >> V >> E;
    cin >> from_node >> to_node;

    Graph g(V, E);
    g.load_graph();

    if(from_node == to_node){
        cout<<0<<'\n'<<from_node;
        return 0;
    }

    vector<int> ans;
    int path_length = bfs(g, ans, from_node, to_node);
    cout << path_length<<'\n';

    if (path_length >= 0) {
        for (int i:ans)
            cout << i << ' ';
    }
    return 0;
}