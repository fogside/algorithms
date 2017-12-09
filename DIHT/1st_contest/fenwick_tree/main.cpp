#include <iostream>
#include "FenwickTree.h"
#include <vector>
#include <assert.h>


int main() {
    vector<int> source_list({1, 2, 5, 6, 7, 3, 2, 1, 3});
    CFenwickTree<int> tree(source_list);
    assert (7 == tree.GetFuncResInSegment(1, 2));
    assert (14 == tree.GetFuncResInSegment(0, 3));
    assert (13 == tree.GetFuncResInSegment(1, 3));
    assert (5 == tree.GetFuncResInSegment(2, 2));

    cout<< tree.GetFuncResInSegment(1, 2)<<'\n';
    cout<< tree.GetFuncResInSegment(0, 3)<<'\n';
    return 0;
}