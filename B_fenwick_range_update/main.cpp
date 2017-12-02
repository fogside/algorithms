#include <iostream>
#include <array>
#include <utility>
#include <algorithm>

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

const u_long len = 1<<24;

array<int, len> values;
array<int, len> deltas;

void add_value(long b, long value) {
    if (b < 0)
        return;
    for (int i = b; i >= 0; i = (i & (i + 1)) - 1)
        deltas[i] += value;

    for (int i = b | (b + 1); i < len; i = i | (i + 1))
        values[i] += value * (b - (i & (i + 1)) + 1);
}

long get_sum(long a) {
    if (a < 0)
        return 0;
    long res = 0;
    for (int i = a; i >= 0; i = (i & (i + 1)) - 1)
        res += values[i] + deltas[i] * (i - (i & (i + 1)) + 1);

    for (int i = a | (a + 1); i < len; i = i | (i + 1))
        res += deltas[i] * (a - (i & (i + 1)) + 1);
    return res;
}

unsigned int nextRand(u_int a, u_int b, u_int &cur) {
    cur = cur * a + b; // вычисляется с переполнениями
    return cur >> 8; // число от 0 до 2^(24-1).
}

int main() {

    unsigned int m, q, a, b; // даны во входных данных
    cin >> m >> q;
    cin >> a >> b;

    unsigned int add = 0, l = 0, r = 0, cur = 0; // беззнаковое 32-битное число

    for (u_long i = 0; i < m; ++i) {
        add = nextRand(a, b, cur); // число, которое нужно прибавить
        l = nextRand(a, b, cur);
        r = nextRand(a, b, cur);
        if (l > r) swap(l, r); // получили отрезок [l..r]
        add_value(r - 1, add);
        add_value(l - 2, -add);
    }

    u_long sum = 0;
    for (u_long i = 0; i < q; ++i) {
        l = nextRand(a, b, cur);
        r = nextRand(a, b, cur);
        if (l > r) swap(l, r); // получили отрезок [l..r]
        sum += get_sum(r - 1);
        if (l >= 1)
            sum -= get_sum(l - 2);
    }

    sum = sum << 32;
    sum = sum >> 32;
    cout << sum << '\n';

    return 0;
}