#include <iostream>
#include "BSTree.h"

using namespace std;

int main()
{
	CBinarySearchTree<int, int> bst;
	for (int i = 0; i < 40; i++) {
		bst.InsertNode(0, rand() % 100);
	}
    for (auto n=bst.Begin(); n!=bst.End(); ++n) {
        std::cout << n.node->value << " ";
    }
	return 0;
}