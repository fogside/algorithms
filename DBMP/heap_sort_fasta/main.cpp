#include <iostream>
#include <fstream>
#include <vector>

/*
 * Сортировка входящего fasta-файла по лексикографическому порядку с помощью кучи.
 *
 */


using namespace std;

const string INPUT_FILE = "/home/fogside/Projects/algorithms/DBMP/heap_sort_fasta/input_example.fasta";
const string OUTPUT_FILE = "/home/fogside/Projects/algorithms/DBMP/heap_sort_fasta/sorted_output.fasta";

//////////////////////////////////////////////////////////////////////////////////////////////////////

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

    ifstream input_file(INPUT_FILE);
    ofstream output_file;
    output_file.open(OUTPUT_FILE);

    vector<char> string_vec;

    string line;
    string tmp_line;

    if (input_file.is_open()) {
        while (getline(input_file, line)) {
            if ((line.front() == '>') && (!string_vec.empty())) {
                heapSort(string_vec);
                output_file << tmp_line << '\n';
                output_file << string(string_vec.begin(), string_vec.end());
                output_file << '\n';
                string_vec.clear();
                tmp_line = line;
            }
            if ((line.front() == '>') && (string_vec.empty())) {
                tmp_line = line;

            } else {
                string_vec.resize(string_vec.size() + line.size());
                for (long i = string_vec.size() - line.size(); i < string_vec.size(); ++i)
                    string_vec[i] = line[i];
            }

        }
    }
    input_file.close();
    output_file.close();
    return 0;
}