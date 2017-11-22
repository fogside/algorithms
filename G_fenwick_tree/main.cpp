#include <iostream>
#include <vector>
#include <cassert>


using namespace std;

class FenwickTree {
public:
    explicit FenwickTree(vector<int> &data_source);

    void update(int index, int new_val, int old_val);

    long get_one_res(int right);

    long get_res(int left, int right);

private:
    vector<long> data;
};

void FenwickTree::update(int index, int new_val, int old_val = 0) {
    if (!old_val) {
        for (; index < data.size(); index = index | (index + 1))
            data[index] += new_val;
    } else {
        int diff = new_val - old_val;
//        printf("new_val: %d, old_val: %d, diff: %d\n", new_val, old_val, diff);
        for (; index < data.size(); index = index | (index + 1)) {
            data[index] += diff;
        }

    }
}

FenwickTree::FenwickTree(vector<int> &data_source) : data(data_source.size()) {
    for (int i = 0; i < data_source.size(); ++i)
        update(i, data_source[i]);
}


long FenwickTree::get_one_res(int right) {
    long res = 0;
    while (right >= 0) {
        res += data[right];
        right = (right & (right + 1)) - 1;
    }
    return res;
}

long FenwickTree::get_res(int left, int right) {
    if (left == 0)
        return get_one_res(right);
    return get_one_res(right) - get_one_res(left - 1);
}


int main() {
    int N, M;
    cin >> N;
    vector<int> source_data(N);
    for (int i = 0; i < N; ++i)
        cin >> source_data[i];

    cin >> M;
    char cmd;
    int a, b;

    FenwickTree tree(source_data);

    for (int i = 0; i < M; ++i) {
        cin >> cmd;
        if (cmd == 's') {
            cin >> a >> b;
            cout << tree.get_res(--a, --b) << ' ';
        }
        if (cmd == 'u') {
            cin >> a >> b;
            --a;
            tree.update(a, b, source_data[a]);
            source_data[a] = b;
        }

    }

    return 0;
}