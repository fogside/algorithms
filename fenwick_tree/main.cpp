#include <iostream>
#include "FenwickTree.h"
#include <vector>


int main() {
    vector<int> source_list = {1, 2, 5, 6, 7, 3, 2, 1, 3};
    CFenwickTree<int> tree(source_list);
    int res = tree.GetFuncResInSegment(1, 2);
    std::cout << "ANS: " << res << '\n';
    return 0;
}