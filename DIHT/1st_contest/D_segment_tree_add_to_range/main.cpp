#include <iostream>
#include <vector>
#include <cmath>
#include <queue>

#define u_int unsigned int
using namespace std;

class SegmentTree {
public:
    explicit SegmentTree(u_int m);

    void update_range(u_int qleft, u_int qright, u_int delta);

    u_int get_value(u_int index);

    //пропихивание значения дельты из root на один уровень ниже
    void _push_delta(u_int root, u_int left, u_int right);

    //присваивание дельты верхнеуровневым нодам
    void _modify(u_int root, u_int cur_left, u_int cur_right, u_int qleft, u_int qright, u_int delta);

    //дальнейшее проталкивание дельт из верхних нод к нижайшим нодам и расчет итогового значения
    u_int _query(u_int root, u_int cur_left, u_int cur_right, u_int qleft, u_int qright);

    u_int n;

private:
    vector<u_int> data;
    vector<u_int> deltas;
};

SegmentTree::SegmentTree(u_int m) : n(2*m), data(2 * n, 0), deltas(2 * n, 0) {
    for (u_int i = this->n + 1; i < m + n + 1; ++i)
        cin >> data[i];

    for (u_int i = n; i > 0; --i)
        data[i] = data[2 * i] + data[2 * i + 1];
}

u_int SegmentTree::get_value(u_int index) {
    return _query(1, 1, n - 1, index, index);
}

void SegmentTree::update_range(u_int qleft, u_int qright, u_int delta) {
    _modify(1, 1, n - 1, qleft, qright, delta);
}

void SegmentTree::_push_delta(u_int root, u_int left, u_int right) {
    deltas[2 * root + 1] += deltas[root];
    deltas[2 * root] += deltas[root];

    int middle = (left + right) / 2;
    data[2 * root + 1] += deltas[root] * (right - middle);
    data[2 * root] += deltas[root] * (middle - left + 1);

    deltas[root] = 0;
}

void SegmentTree::_modify(u_int root, u_int cur_left, u_int cur_right, u_int qleft, u_int qright, u_int delta) {
    if (qright < cur_left || qleft > cur_right)
        return;

    //если все дерево текущее внутри интервала запроса
    if (qleft <= cur_left && cur_right <= qright) {
        deltas[root] += delta;
        data[root] += delta * (cur_right - cur_left + 1);
        return;
    }
    //а если нет, то продолжаем сужать интервал для cur_left/cur_right
    //не забыв в текущий рут добавить значение дельты
    _push_delta(root, cur_left, cur_right);

    //спуск в левое поддерево
    _modify(2 * root, cur_left, (cur_left + cur_right) / 2, qleft, qright, delta);

    //спуск в правое поддерево
    _modify(2 * root + 1, (cur_left + cur_right) / 2 + 1, cur_right, qleft, qright, delta);

    //обновляем значение в текущей ноде, зная что ниже все уже обновилось
    deltas[root] = data[2 * root + 1] + data[2 * root];
}

u_int SegmentTree::_query(u_int root, u_int cur_left, u_int cur_right, u_int qleft, u_int qright) {

    if (qright < cur_left || qleft > cur_right)
        return 0;

    //если все дерево текущее лежит в отрезке запроса, то возвращаем значение в текущем корне
    if (qleft <= cur_left && cur_right <= qright)
        return data[root];

    //иначе пропихиваем дельты от корня к детям и все суммируем
    _push_delta(root, cur_left, cur_right);
    return _query(root * 2, cur_left, (cur_left + cur_right) / 2, qleft, qright) +
           _query(root * 2 + 1, (cur_left + cur_right) / 2 + 1, cur_right, qleft, qright);
}


int main() {
    u_int N;
    char cmd;

    cin >> N;
    SegmentTree tree(N);

    u_int K, left, right, index, value;
    cin >> K;
    for (int i = 0; i < K; ++i) {
        cin >> cmd;
        if (cmd == 'g') {
            cin >> index;
            cout << tree.get_value(index) << '\n';
        }
        if (cmd == 'a') {
            cin >> left >> right >> value;
            tree.update_range(left, right, value);
        }
    }
    return 0;
}
