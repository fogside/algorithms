#include <iostream>
#include <vector>

/*
Есть массив целых чисел длины n = 2^24, изначально заполненных нулями.
Вам нужно сперва обработать m случайных запросов вида "прибавление на отрезке".
Затем обработать q случайных запросов вида "сумма на отрезке".
 ## Input format ##
На первой строке числа m, q (1 ≤ m, q ≤ 2^24).
На второй строке пара целых чисел a, b от 1 до 10^9, используемая в генераторе случайных чисел.
    unsigned int a, b; // даны во входных данных
    unsigned int cur = 0; // беззнаковое 32-битное число
    unsigned int nextRand() {
      cur = cur * a + b; // вычисляется с переполнениями
      return cur >> 8; // число от 0 до 2^(24-1).
    }
Каждый запрос первого вида генерируется следующим образом:
    add_value = nextRand(); // число, которое нужно прибавить
    l = nextRand();
    r = nextRand();
    if (l > r) swap(l, r); // получили отрезок [l..r]
Каждый запрос второго вида генерируется следующим образом:
    l = nextRand();
    r = nextRand();
    if (l > r) swap(l, r); // получили отрезок [l..r]
Сперва генерируются запросы первого вида, затем второго.
## Output format ##
Выведите сумму ответов на все запросы второго типа по модулю 2^32.
 */

using namespace std;

const u_int LENGTH = 1 << 24;

unsigned int a, b; // даны во входных данных
unsigned int cur = 0; // беззнаковое 32-битное число
unsigned int nextRand() {
    cur = cur * a + b; // вычисляется с переполнениями
    return cur >> 8; // число от 0 до 2^24-1.
}


int main() {

    u_int add, l, r, m, q;
    vector<int> input_query(LENGTH, 0);
    vector<int> values(LENGTH, 0);

    cin >> m >> q;
    cin >> a >> b;

    //здесь запоминаем на концах отрезков значения
    //которые далее протянем по всему массиву values;
    for (int i = 0; i < m; ++i) {
        add = nextRand(); // число, которое нужно прибавить
        l = nextRand();
        r = nextRand();
        if (l > r) swap(l, r); // получили отрезок [l..r]
        input_query[l] += add;
        input_query[r + 1] -= add;
    }

    //здесь считаем все, что запрашивали,
    //размазывая по массиву values все инпуты;
    values[0] = input_query[0];
    int cum_sum = values[0];
    for (int i = 1; i < LENGTH; ++i) {
        cum_sum += input_query[i];
        values[i] = cum_sum + values[i - 1];
    }

    //отвечаем на запросы
    u_long sum = 0;
    for (int i = 0; i < q; ++i) {
        l = nextRand();
        r = nextRand();
        if (l > r) swap(l, r); // получили отрезок [l..r]
        sum += values[r];
        if (l != 0)
            sum -= values[l - 1];
    }
    sum = sum << 32;
    sum = sum >> 32;
    cout << sum << '\n';

    return 0;
}