#include <iostream>
#include <vector>

/*
 * Сортировка входящего fasta-файла по лексикографическому порядку с помощью кучи.
 *
 */


using namespace std;

void heapify(vector<char> &arr, long size, long parent_num) {

    long max_val_idx = parent_num;
    long left = parent_num * 2 + 1;
    long right = parent_num * 2 + 2;

    if ((left >= size) && (right >= size))
        return;

    if ((left < size) && (arr[left] > arr[parent_num]))
        max_val_idx = left;

    if ((right < size) && (arr[right] > arr[max_val_idx]))
        max_val_idx = right;

    if (max_val_idx != parent_num) {
        swap(arr[parent_num], arr[max_val_idx]);
        heapify(arr, size, max_val_idx);
    }
}

void build_heap(vector<char> &arr, u_long size) {
    for (long i = size / 2 - 1; i >= 0; --i) {
        heapify(arr, size, i);
    }
}

void heapSort(vector<char> &arr) {
    u_long size = arr.size();

    build_heap(arr, size);

    while (size >= 1) {
        swap(arr[0], arr[size - 1]);
        --size;
        heapify(arr, size, 0);
    }
}


int main() {
    vector<char> lol = {'a', 't', 'g', 'c', 'a', 'a', 'a', 'b', 'e', 'g', 'h'};
    //TODO: прикрутить чтение и запись в fasta file;
    heapSort(lol);
    for (char i: lol)
        cout << i << ' ';
    return 0;
}