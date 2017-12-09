#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

/*
 * Вам даны пары чисел (a_i, b_i),
 * Вам необходимо построить декартово дерево, такое что i-ая вершина имеет ключи (a_i, b_i),
 * вершины с ключом a_i образуют бинарное дерево поиска, а вершины с ключом b_i образуют кучу.

Input format

    В первой строке записано число N — количество пар.
    Далее следует N (1 ≤ N ≤ 50000) пар (a_i, b_i).
    Для всех пар |a_i|, |b_i| ≤ 30000 . a_i ≠ a_j и b_i ≠ b_j для всех i ≠ j.

Output format

    Если декартово дерево с таким набором ключей построить возможно,
    выведите в первой строке YES, в противном случае выведите NO.
    В случае ответа YES, выведите N строк,
    каждая из которых должна описывать вершину.
    Описание вершины состоит из трёх чисел:
     номер предка,
     номер левого сына
     и номер правого сына.
    Если у вершины отсутствует предок или какой-либо из сыновей, то выводите на его месте число 0.

    Если подходящих деревьев несколько, выведите любое.
 */

using namespace std;

class Treap {
public:
    int x;
    int y;
    int number;

    Treap *left;
    Treap *right;

    friend Treap *merge(Treap *L, Treap *R);

    friend void output(Treap *t, vector<int> &parent, vector<int> &left, vector<int> &right, int parent_num);

    Treap(int key, int priority, int number) : x(key), y(priority), number(number), left(nullptr), right(nullptr) {};

    Treap() : left(nullptr), right(nullptr), x(0), y(0), number(0) {};

    ~Treap();
};

struct cmp_obj {
    bool operator()(const Treap &t1, const Treap &t2) {
        return t1.x > t2.x;
    }
} comparator;

Treap::~Treap() {
//    delete left;
//    delete right;
}

Treap *merge(Treap *L, Treap *R) {
    // корень -- вершина с наибольшим приоритетом;
    // ключи L < ключей R;

    if (L == nullptr)
        return R;
    if (R == nullptr)
        return L;
    if (L->y < R->y) {
        L->right = merge(L->right, R);
        return L;
    } else {
        R->left = merge(L, R->left);
        return R;
    }
}

void output(Treap *t, vector<int> &parent, vector<int> &left, vector<int> &right, int parent_num) {
    if (t == nullptr)
        return;

    if (t->right != nullptr)
        right[t->number] = t->right->number;
    else
        right[t->number] = 0;

    if (t->left != nullptr)
        left[t->number] = t->left->number;
    else
        left[t->number] = 0;

    parent[t->number] = parent_num;

    output(t->right, parent, left, right, t->number);
    output(t->left, parent, left, right, t->number);
}


int main() {

    int n, key, priority;
    std::cin >> n;
    std::vector<Treap> treaps(n + 1); //нумерация с 1;
    for (int i = 1; i <= n; ++i) {
        cin >> key >> priority;
        treaps[i] = Treap(key, priority, i);
    }
    //сортируем по убыванию ключей
    sort(treaps.begin() + 1, treaps.end(), comparator);
    queue<Treap *, deque<Treap *>> q;
    for (int i = 1; i < treaps.size(); i++) {
        q.push(&treaps[i]);
    }

    vector<int> left(n + 1), right(n + 1), parent(n + 1);

    Treap *t1, *t2, *t;
    while (!q.empty()) {
        t1 = q.front();
        q.pop();
        t2 = q.front();
        q.pop();
        t = merge(t2, t1);
        if (!q.empty())
            q.push(t);
    }

    output(t, parent, left, right, 0);
    cout<<"YES\n";

    for (int i = 1; i <= n; ++i)
        cout <<parent[i]<<' '<<left[i]<<' '<<right[i]<<'\n';
}