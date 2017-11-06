#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

class SparseTable {
private:
    vector<vector<long> > table;
    vector<int> K;
public:
    explicit SparseTable(vector<long> &data);

    long RMQ(unsigned long u, unsigned long v);
};

SparseTable::SparseTable(vector<long> &data) : table(int(log2(data.size())) + 1, vector<long>(data.size())),
                                               K(data.size()) {

    for (int j = 1; j < data.size(); ++j){
        K[j] = int(log2(j));
        table[0][j] = data[j];
    }

    for (int k = 1; k < table.size(); ++k) {
        unsigned long p = pow(2, k - 1);
        for (unsigned long i = 1; i + p <= data.size(); ++i)
            table[k][i] = min(table[k - 1][i], table[k - 1][i + p]);
    }
}

long SparseTable::RMQ(unsigned long u, unsigned long v) {
    if (u > v)
        swap(u, v);

    int k = K[v - u + 1];
    return min(table[k][u], table[k][v - pow(2, k) + 1]);
}


int main() {
    unsigned long n, m, v1, u1;
    int a1;
    cin >> n >> m >> a1;
    cin >> u1 >> v1;

    // data formation
    vector<long> data(n + 1);
    data[1] = a1;
    for (int i = 2; i <= n; ++i)
        data[i] = (23 * data[i - 1] + 21563) % 16714589;

    // ST formation
    SparseTable sparseTable(data);

    // requests formation
    //ui + 1 = (17 · ui + 751 + ansi + 2i) % n + 1,
    // vi + 1 = (13 · vi + 593 + ansi + 5i) % n + 1
    long ans = sparseTable.RMQ(u1, v1);
    for (int i = 2; i <= m; ++i) {
        u1 = (17 * u1 + 751 + ans + 2 * (i - 1)) % n + 1;
        v1 = (13 * v1 + 593 + ans + 5 * (i - 1)) % n + 1;
        ans = sparseTable.RMQ(u1, v1);
    }
    cout << u1 << ' ' << v1 << ' ' << ans;
    return 0;
}