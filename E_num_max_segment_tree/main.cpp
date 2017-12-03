#include <iostream>
#include <vector>
#include <cmath>
#include <limits.h>
#include <tuple>

/*
 *
 * Реализуйте структуру данных для эффективного вычисления
 * значения максимального из нескольких подряд идущих элементов массива,
 * а также количества элементов, равных максимальному на данном отрезке.

 # Input format #

    В первой строке вводится одно натуральное число N(1 ≤ N ≤ 105) - количество чисел в массиве.

    Во второй строке вводятся N чисел от 1 до 100000 - элементы массива.

    В третьей строке вводится одно натуральное число K(1 ≤ K ≤ 30000) — количество запросов на вычисление максимума.

    В следующих K строках вводится по два числа - номера левого и правого элементов отрезка массива
    (считается, что элементы массива нумеруются с единицы).

 # Output format #
    Для каждого запроса выведите в отдельной строке через пробел
    значение максимального элемента на указанном отрезке массива
    и количество максимальных элементов на этом отрезке.
 */


using namespace std;

const int MIN_VAL = INT32_MIN;

class SegmentTree {
public:
    explicit SegmentTree(unsigned int m);

    pair<u_int, u_int> get_res_on_segment(unsigned int left, unsigned int right);

    unsigned int n;

private:
    vector<int> data;
    vector<int> num_max;
};

pair<int, int> combine(int p1_val, int p1_num, int p2_val, int p2_num) {
    if (p1_val > p2_val)
        return make_pair(p1_val, p1_num);
    if (p2_val > p1_val)
        return make_pair(p2_val, p2_num);
    return make_pair(p1_val, p1_num + p2_num);
};

SegmentTree::SegmentTree(unsigned int m) : n(1 << (ilogb(m - 1) + 1)),
                                           data(2 * n - 1, MIN_VAL),
                                           num_max(2 * n - 1, 0) {
    for (unsigned int i = this->n; i < m + n; ++i){
        cin >> data[i];
        num_max[i]=1;
    }

    for (unsigned int i = n - 1; i > 0; --i)
            tie(data[i], num_max[i]) = combine(data[2 * i], num_max[2 * i],
                          data[2 * i + 1], num_max[2 * i + 1]);
}

pair<u_int, u_int> SegmentTree::get_res_on_segment(unsigned int left, unsigned int right) {
    left += n-1;
    right += n-1;

    int max_val = MIN_VAL;
    int num_max_val = 0;

    while (left <= right) {
            // если right - левый сын своего родителя,
            // учитываем его фундаментальный отрезок
        if (!(right & 1))
            tie(max_val, num_max_val) = combine(data[right], num_max[right], max_val, num_max_val);

            // если left - правый сын своего родителя,
            // учитываем его фундаментальный отрезок
        if (left & 1)
            tie(max_val, num_max_val) = combine(data[left], num_max[left], max_val, num_max_val);
        // сдвигаем указатели на уровень выше
        left = (left + 1) / 2;
        right = (right - 1) / 2;
    }

    return make_pair(max_val, num_max_val);
}

int main() {
    unsigned int N;
    cin >> N;
    SegmentTree tree(N);
    unsigned int K, left, right;
    cin >> K;
    pair<int, int> tmp;
    for (int i = 0; i < K; ++i) {
        cin >> left >> right;
        tmp = tree.get_res_on_segment(left, right);
        cout<<tmp.first<<' '<<tmp.second<<'\n';
    }
    return 0;
}