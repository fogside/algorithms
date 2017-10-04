//
// Created by Eugenia Cheskidova on 01/10/2017.
//

#ifndef FENWICK_TREE_FENWICKTREE_H
#define FENWICK_TREE_FENWICKTREE_H

#include <algorithm>
#include <vector>

using namespace std;


template<class T>
class CFenwickTree {
public:
    explicit CFenwickTree(int size = 0) : data(size) {};

    explicit CFenwickTree(const vector<T> &inputArray);

    void Update(int index, const T &value);

    T GetFuncResInSegment(int left, int right);

private:
    vector<T> data;
};

//-------------------------------------------//

template<class T>
CFenwickTree<T>::CFenwickTree(const vector<T> &inputArray) {

    data.resize(inputArray.size());
    for (int i = 0; i < inputArray.size(); ++i)
        Update(i, inputArray[i]);
}

template<class T>
void CFenwickTree<T>::Update(int index, const T &value) {
    long N = data.size();
    for (int i = 0; i < N; ++i) {
        data[index] += value;
        index = index | (index + 1);
    }
}

template<class T>
T CFenwickTree<T>::GetFuncResInSegment(int left, int right) {
    T leftRes = 0;
    T rightRes = 0;

    if(left>0)
        left -= 1;
    while (left >= 0) {
        leftRes += data[left];
        left = (left & (left + 1)) - 1;
    }
    while (right > 0) {
        rightRes += data[right];
        right = (right & (right + 1)) - 1;
    }
    return rightRes - leftRes;
}

#endif //FENWICK_TREE_FENWICKTREE_H
