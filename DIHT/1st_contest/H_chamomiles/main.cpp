#include <iostream>
#include <vector>

// 2D FENWICK TREE

using namespace std;

class FenwickTree2D {
public:
    explicit FenwickTree2D(int nrows, int ncols) : data(nrows, vector<long>(ncols)) {};

    void update(int x, int y, int value);

    long get_one_res(int x1, int y1);

    long get_res(int x1, int y1, int x2, int y2);

    void print_mtx();

private:
    vector<vector<long> > data;
};

void FenwickTree2D::update(int x, int y, int value) {
    for (; x < data.size(); x = x | (x + 1))
        for (int j = y; j < data[0].size(); j = j | (j + 1))
            data[x][j] += value;
}


long FenwickTree2D::get_one_res(int x1, int y1) {
    if((x1<0)||(y1<0))
        return 0;
    long res = 0;
    for (; x1 >= 0; x1 = (x1 & (x1 + 1)) - 1)
        for (int j = y1; j >= 0; j = (j & (j + 1)) - 1)
            res += data[x1][j];
    return res;
}

long FenwickTree2D::get_res(int x1, int y1, int x2, int y2) {
    --x1, --y1;
    long res_22 = get_one_res(x2, y2);
    long res_21 = get_one_res(x2, y1);
    long res_12 = get_one_res(x1, y2);
    long res_11 = get_one_res(x1,y1);
    return res_22 - res_21 - res_12 + res_11;
}

void FenwickTree2D::print_mtx() {
    for(int i = 0; i<data.size(); ++i) {
        for (int j = 0; j<data[0].size(); ++j)
            cout<<data[i][j];
        cout<<'\n';
    }
}


int main() {
    int nrow, ncol, n, x1, y1, x2, y2, val;
    cin >> nrow >> ncol;
    FenwickTree2D tree(nrow, ncol);
    cin >> n;
    int cmd;
    for (int i = 0; i < n; ++i) {
        cin >> cmd;
        if (cmd == 1) {
            cin >> x1 >> y1 >> val;
            --x1, --y1;
            tree.update(x1, y1, val);
        }
        if (cmd == 2) {
            cin >> x1 >> y1 >> x2 >> y2;
            --x1, --y1, --x2, --y2;
            cout << tree.get_res(x1, y1, x2, y2)<<'\n';
        }
    }

    return 0;
}

