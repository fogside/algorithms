#include <iostream>
#include <vector>

using namespace std;

class FenwickTree {
public:
    explicit FenwickTree(vector<long> &source_arr);

    void add_to_range(long a, long b, long value);

    long get_element(long a);

    long len;
private:

    // прибавить значение value на отрезке [0, b]
    void _add(long b, int value);

    // получить сумму элементов на отрезке [0, a]
    long _sum(long a);

    vector<long> values;
    vector<long> deltas;
};

FenwickTree::FenwickTree(vector<long> &source_arr) :
        len(source_arr.size()), values(len, 0), deltas(len, 0) {

    for (int i = 0; i < len; ++i) {
        values[i] += source_arr[i];
        if ((i | (i + 1)) < len)
            values[i | (i + 1)] += values[i];
    }
}

void FenwickTree::add_to_range(long a, long b, long value) {
    _add(b, value);
    _add(a - 1, -value);
}

long FenwickTree::get_element(long a) {
    if (a == 0)
        return _sum(a);

    long s1 = _sum(a);
    long s2 = _sum(a - 1);
    return s1 - s2;
}

void FenwickTree::_add(long b, int value) {
    if (b < 0)
        return;
    for (int i = b; i >= 0; i = (i & (i + 1)) - 1)
        deltas[i] += value;

    for (int i = b | (b + 1); i < len; i = i | (i + 1))
        values[i] += value * (b - (i & (i + 1)) + 1);
}

long FenwickTree::_sum(long a) {
    if (a < 0)
        return 0;
    long res = 0;
    for (int i = a; i >= 0; i = (i & (i + 1)) - 1)
        res += values[i] + deltas[i] * (i - (i & (i + 1)) + 1);

    for (int i = a | (a + 1); i < len; i = i | (i + 1))
        res += deltas[i] * (a - (i & (i + 1)) + 1);
    return res;
}

int main() {


    long N;
    char cmd;

    cin >> N;
    vector<long> arr(N, 0);
    for (long i = 0; i < N; ++i)
        cin >> arr[i];

    FenwickTree tree(arr);

    long K, left, right, index, value;
    cin >> K;
    for (long i = 0; i < K; ++i) {
        cin >> cmd;
        if (cmd == 'g') {
            cin >> index;
            cout << tree.get_element(--index) << '\n';
        }
        if (cmd == 'a') {
            cin >> left >> right >> value;
            tree.add_to_range(--left, --right, value);
        }
    }
    return 0;
}