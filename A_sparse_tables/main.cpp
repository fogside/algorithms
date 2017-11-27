#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

class SparseTable {
public:

    vector<vector<long> > table;
    vector<int> K;

    explicit SparseTable(int a1, long n);

};

SparseTable::SparseTable(int a1, long n) : table((ilogb(n + 1)) + 1, vector<long>(n + 1)), K(n + 1) {

    table[0][1] = a1;
    for (long i = 2; i <= n; ++i) {
        table[0][i] = (23 * table[0][i - 1] + 21563) % 16714589;
        K[i] = ilogb(i);
    }

    for (int k = 1; k < table.size(); ++k) {
        unsigned long p = 1<<(k - 1);
        for (unsigned long i = 1; i + p <= n + 1; ++i)
            table[k][i] = min(table[k - 1][i], table[k - 1][i + p]);
    }
}


int main() {
    unsigned long n, m, v1, u1;
    int a1;
    bool FLAG = false;

    cin >> n >> m >> a1;
    cin >> u1 >> v1;

    SparseTable sparseTable(a1, n);


    // govnokod

    if (u1 > v1) {
        swap(u1, v1);
        FLAG = true;
    }

    int k = sparseTable.K[v1 - u1 + 1];
    int power = 1 << k;
    long ans = min(sparseTable.table[k][u1], sparseTable.table[k][v1 - power + 1]);

    if (FLAG) {
        swap(u1, v1);
        FLAG = false;
    }

    for (int i = 2; i <= m; ++i) {
        u1 = (17 * u1 + 751 + ans + 2 * (i - 1)) % n + 1;
        v1 = (13 * v1 + 593 + ans + 5 * (i - 1)) % n + 1;
        if (u1 > v1) {
            swap(u1, v1);
            FLAG = true;
        }

        k = sparseTable.K[v1 - u1 + 1];
        power = 1 << k;
        ans = min(sparseTable.table[k][u1], sparseTable.table[k][v1 - power + 1]);
        if (FLAG) {
            swap(u1, v1);
            FLAG = false;
        }
    }
    cout << u1 << ' ' << v1 << ' ' << ans;
    return 0;
}