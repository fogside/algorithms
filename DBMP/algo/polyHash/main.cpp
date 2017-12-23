#include <iostream>
#include <fstream>
#include <vector>

/*
 * На вход программе подаётся референсная строка и fasta-файл с ридами произвольной длины.
 * Необходимо найти все вхождения ридов в референсную последовательность, используя полиномиальные хеши.
 */


#define K 11
using namespace std;

void compute_powers(vector<unsigned long int> &powers) {
    powers[0] = 1;
    for (int i = 1; i < powers.size(); i++)
        powers[i] = powers[i - 1] * K;
}

void find_occurences(string &read, u_long ref_size,
                     vector<unsigned long int> &ref_hashes,
                     vector<unsigned long int> &powers) {

    unsigned long int prefix_hash;
    unsigned long int read_hash;
    bool found = false;

    // Compute hash for read
    read_hash = 0;
    for (int i = 0; i < read.size(); i++)
        read_hash += powers[i] * (long int) read[i];

    cout << "FOR READ: " << read << '\n';
    // Compare hashes. NOTE: for many reads you should precompute prefix hashes:
    // ref_hashes[i + read.size() - 1] - ref_hashes[i-1]
    if (read_hash == ref_hashes[read.size() - 1]) {
        found = true;
        printf("Read found at position: 0");
    }
    for (int i = 1; i < ref_size - read.size() + 1; i++) {
        prefix_hash = ref_hashes[i + read.size() - 1] - ref_hashes[i - 1];
        if (prefix_hash == read_hash * powers[i]) {
            found = true;
            printf("Read found at position: %i\n", i);
        }
    }
    if(!found)
        cout<<"Nothing found!"<<'\n';
    cout << '\n';
}

int main() {
    const string reference_path = "/home/fogside/Projects/algorithms/DBMP/algo/polyHash/reference.txt";
    const string read_path = "/home/fogside/Projects/algorithms/DBMP/algo/polyHash/reads.txt";

    string reference;
    vector<string> reads;

    unsigned long int prefix_hash;
    unsigned long int read_hash;


    ifstream ref_file(reference_path);
    ifstream reads_file(read_path);

    if (ref_file.is_open()) {
        getline(ref_file, reference);
    } else {
        cout << "Can't open ref file!" << '\n';
        return 1;
    }

    if (reads_file.is_open()) {
        string r;
        while (getline(reads_file, r)) {
            reads.push_back(r);
        }
    } else {
        cout << "Can't open reads file!" << '\n';
        return 1;
    }

    ref_file.close();
    reads_file.close();


    vector<unsigned long int> powers(reference.size());
    vector<unsigned long int> ref_hashes(reference.size());

    // Precompute all powers
    compute_powers(powers);

    // Compute hashes for reference prefixes
    ref_hashes[0] = (unsigned long int) reference[0];
    for (int i = 1; i < reference.size(); i++)
        ref_hashes[i] = (long int) reference[i] * powers[i] + ref_hashes[i - 1];

    for (auto read: reads) {
        find_occurences(read, reference.size(), ref_hashes, powers);
    }
}