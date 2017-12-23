#include <iostream>
#include <fstream>
#include <vector>

/*
 * Сортировка входящего fasta-файла по лексикографическому порядку с помощью кучи.
 *
 */


using namespace std;

const string INPUT_FILE = "/home/fogside/Projects/algorithms/DBMP/algo/heap_sort_fasta/input_example.fasta";
const string OUTPUT_FILE = "/home/fogside/Projects/algorithms/DBMP/algo/heap_sort_fasta/sorted_output.fasta";

//////////////////////////////////////////////////////////////////////////////////////////////////////

void heapify(vector<pair<string, string>> &arr, long size, long parent_num) {

    long max_val_idx = parent_num;
    long left = parent_num * 2 + 1;
    long right = parent_num * 2 + 2;

    if ((left >= size) && (right >= size))
        return;

    if ((left < size) && (arr[left].first > arr[parent_num].first))
        max_val_idx = left;

    if ((right < size) && (arr[right].first > arr[max_val_idx].first))
        max_val_idx = right;

    if (max_val_idx != parent_num) {
        swap(arr[parent_num], arr[max_val_idx]);
        heapify(arr, size, max_val_idx);
    }
}

void build_heap(vector<pair<string, string>> &arr, u_long size) {
    for (long i = size / 2 - 1; i >= 0; --i) {
        heapify(arr, size, i);
    }
}

void heapSort(vector<pair<string, string>> &arr) {
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

    // pair.first -- seq;
    // pair.second -- fasta name;
    vector<pair<string, string>> seq_list;

    string line;
    string name;
    long tmp_size;

    if (input_file.is_open()) {
        while (getline(input_file, line)) {
            if (line.front() == '>') {
                name = line;
                getline(input_file, line);
                seq_list.emplace_back(make_pair(line, name));

            } else {
                // the case of multiline fasta sequence
                tmp_size = seq_list.back().first.size();
                seq_list.back().first.resize(tmp_size + line.size());
                for (long i = 0; i < line.size(); ++i)
                    seq_list.back().first[i+tmp_size] = line[i];
            }

        }

    }
    heapSort(seq_list);
    input_file.close();

    for (auto i : seq_list) {
        output_file<<i.second<<'\n';
        output_file<<i.first<<'\n';
    }

    output_file.close();
    return 0;
}