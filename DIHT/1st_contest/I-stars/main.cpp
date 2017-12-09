#include <iostream>
#include <vector>

// 3D FENWICK TREE

using namespace std;

class FenwickTree3D {
public:
    explicit FenwickTree3D(int x, int y, int z) : data(x, vector<vector<long> >(y, vector<long>(z))) {};

    void update(int x, int y, int z, int value);

    long get_one_res(int x1, int y1, int z1);

    long get_res(int x1, int y1, int z1, int x2, int y2, int z2);

private:
    vector<vector<vector<long> > > data;
};

void FenwickTree3D::update(int x, int y, int z, int value) {
    for (; x < data.size(); x = x | (x + 1))
        for (int j = y; j < data[0].size(); j = j | (j + 1))
            for (int i = z; i < data[0][0].size(); i = i | (i + 1))
                data[x][j][i] += value;
}


long FenwickTree3D::get_one_res(int x1, int y1, int z1) {
    if ((x1 < 0) || (y1 < 0) || (z1 < 0))
        return 0;
    long res = 0;
    for (; x1 >= 0; x1 = (x1 & (x1 + 1)) - 1)
        for (int j = y1; j >= 0; j = (j & (j + 1)) - 1)
            for (int i = z1; i >= 0; i = (i & (i + 1)) - 1)
                res += data[x1][j][i];
    return res;
}

long FenwickTree3D::get_res(int x1, int y1, int z1, int x2, int y2, int z2) {

    return get_one_res(x2, y2, z2)
           - get_one_res(x1 - 1, y2, z2)
           - get_one_res(x2, y1 - 1, z2)
           - get_one_res(x2, y2, z1 - 1)
           + get_one_res(x2, y1 - 1, z1 - 1)
           + get_one_res(x1 - 1, y2, z1 - 1)
           + get_one_res(x1 - 1, y1 - 1, z2)
           - get_one_res(x1 - 1, y1 - 1, z1 - 1);
}


int main() {
    int n, x1, y1, z1, x2, y2, z2, val;
    cin >> n;
    FenwickTree3D tree(n, n, n);
    int cmd;
    while (true) {
        cin >> cmd;
        if (cmd == 1) {
            cin >> x1 >> y1 >> z1 >> val;
            tree.update(x1, y1, z1, val);
        }
        if (cmd == 2) {
            cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;
            cout << tree.get_res(x1, y1, z1, x2, y2, z2) << '\n';
        }
        if (cmd == 3)
            break;
    }
    return 0;
}
