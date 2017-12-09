#include <iostream>
#include <algorithm>

#ifndef BSTREE_BSTREE_H
#define BSTREE_BSTREE_H

using namespace std;

template<class K, class V, class Cmp = less<K> >
class CBinarySearchTree {
public:
    CBinarySearchTree() = default;

    CBinarySearchTree(const CBinarySearchTree<K, V, Cmp> &_bst) = delete;

    struct CNode {
        CNode() = default;

        CNode(const K &_key, const V &_value) :
                key(_key), value(_value) {}

        CNode *parent = nullptr;
        CNode *left = nullptr;
        CNode *right = nullptr;
        K key = K();
        V value = V();
    };

    class CIterator : public iterator<forward_iterator_tag, CNode> {
    public:
//        CIterator(const CIterator &iter) = default;
        CIterator() = default;

        CNode *node = nullptr;
        CBinarySearchTree *tree = nullptr;

        CIterator &operator++() {
            if (node == nullptr) {
                return *this;
            }

            CNode *temp = node;
            node = node->parent;
            if (node == nullptr || node->right == temp) {
                return *this;
            }
            node = node->right;
            while (node->left != nullptr || node->right != nullptr) {
                if (node->left != nullptr) {
                    node = node->left;
                    continue;
                }
                node = node->right;
            }
            return *this;
        }

        bool operator==(const CIterator &iter) const {
            return iter.node == node;
        }

        bool operator!=(const CIterator &iter) const {
            return iter.node != node;
        }

//        CNode* operator // what does it mean?
    };

    void InsertNode(const K &key, const V &value);

    CIterator Begin() {
        if (root == nullptr) {
            return End();
        }
        CNode *node = root;
        while (node->left != nullptr || node->right != nullptr) {
            if (node->left != nullptr) {
                node = node->left;
                continue;
            }
            node = node->right;
        }
        CIterator iter;
        iter.node = node;
        iter.tree = this;
        return iter;
    }

    CIterator End() {
        CIterator iter;
        iter.tree = this;
        return iter;
    }

private:
    CNode *root = nullptr;
    int size = 0;
    Cmp comparator = Cmp();

    CNode *insert(CNode *&node, const K &key, const V &value);
};


template<class K, class V, class Cmp>
using CBST = CBinarySearchTree<K, V, Cmp>;

template<class K, class V, class Cmp>
using CBSTNode = typename CBinarySearchTree<K, V, Cmp>::CNode;

template<class K, class V, class Cmp>
using CBSTIter = typename CBinarySearchTree<K, V, Cmp>::CIterator;

template<class K, class V, class Cmp>
void CBST<K, V, Cmp>::InsertNode(const K &key, const V &value) {
    insert(root, key, value);
    size++;
}

template<class K, class V, class Cmp>
CBSTNode<K, V, Cmp> *CBST<K, V, Cmp>::insert(
         CBSTNode<K, V, Cmp> *&node, const K &key, const V &value) {
    if (node == nullptr) {
        node = new CNode(key, value);
    } else if (comparator(key, node->key)) {
        node->left = insert(node->left, key, value);
        node->left->parent = node;
    } else {
        node->right = insert(node->right, key, value);
        node->right->parent = node;
    }
    return node;
}

#endif //BSTREE_BSTREE_H
