#include <iostream>
#include <vector>
#include <cmath>
#include <queue>

using namespace std;

unsigned int gcd(unsigned int a, unsigned int b) {
    while (b) {
        a %= b;
        swap(a, b);
    }
    return a;
}

class SegmentTree {
public:
    explicit SegmentTree(unsigned int m);

    unsigned int get_gcd_on_segment(unsigned int left, unsigned int right);

    unsigned int n;

private:
    vector<unsigned int> data;
};

SegmentTree::SegmentTree(unsigned int m) : n(1 << (ilogb(m - 1) + 1)), data(2 * n - 1, 1) {
    for (unsigned int i = this->n; i < m + n; ++i)
        cin >> data[i];

    for (unsigned int i = n - 1; i > 0; --i)
        data[i] = gcd(data[2 * i], data[2 * i + 1]);
}

unsigned int SegmentTree::get_gcd_on_segment(unsigned int left, unsigned int right) {
    left += n-1;
    right += n-1;
    queue<unsigned int>chosen_values;

    while (left <= right) {
        if ((left & 1) && !(right & 1))
            chosen_values.push(gcd(data[left], data[right]));

            // если right - левый сын своего родителя,
            // учитываем его фундаментальный отрезок
        else if (!(right & 1))
            chosen_values.push(data[right]);
            // если left - правый сын своего родителя,
            // учитываем его фундаментальный отрезок
        else if (left & 1)
            chosen_values.push(data[left]);
        // сдвигаем указатели на уровень выше
        left = (left + 1) / 2;
        right = (right - 1) / 2;
    }

    unsigned int ans = chosen_values.front();
    chosen_values.pop();
    while (!chosen_values.empty()){
        ans = gcd(ans, chosen_values.front());
        chosen_values.pop();
    }
    return ans;
}

int main() {
    unsigned int N;
    cin >> N;
    SegmentTree tree(N);
    unsigned int K, left, right;
    cin >> K;
    for (int i = 0; i < K; ++i) {
        cin >> left >> right;
        cout << tree.get_gcd_on_segment(left, right) << '\n';
    }
    return 0;
}