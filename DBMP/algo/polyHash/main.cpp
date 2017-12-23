#include <iostream>
#include <fstream>
#include <vector>

#define K 11

using namespace std;

void compute_powers(vector<unsigned long int> &powers);

int main() {
    const string INPUT_FILE = "/home/fogside/Projects/algorithms/DBMP/algo/polyHash/reference.txt";
    string reference;
    string read = "TLEN";

    unsigned long int prefix_hash;
    unsigned long int read_hash;


    ifstream input_file(INPUT_FILE);
    if (input_file.is_open()) {
        getline(input_file, reference);
    } else {
        cout << "Can't open input file!" << '\n';
        return 1;
    }

    vector<unsigned long int> powers(reference.size());
    vector<unsigned long int> ref_hashes(reference.size());

    // Precompute all powers of the
    compute_powers(powers);

    // Compute hashes for reference prefixes
    ref_hashes[0] = (unsigned long int) reference[0];
    for (int i = 1; i < reference.size(); i++)
        ref_hashes[i] = (long int) reference[i] * powers[i] + ref_hashes[i - 1];


    // Compute hash for read
    read_hash = 0;
    for (int i = 0; i < read.size(); i++)
        read_hash += powers[i] * (long int) read[i];

    // Compare hashes. NOTE: for many reads you should precompute prefix hashes:
    // ref_hashes[i + read.size() - 1] - ref_hashes[i-1]
    if (prefix_hash == ref_hashes[read.size() - 1])
        printf("Read found at position: 0");
    for (int i = 1; i < reference.size() - read.size() + 1; i++) {
        prefix_hash = ref_hashes[i + read.size() - 1] - ref_hashes[i - 1];
        if (prefix_hash == read_hash * powers[i])
            printf("Read found at position: %i\n", i);
    }
}

void compute_powers(vector<unsigned long int> &powers) {
    powers[0] = 1;
    for (int i = 1; i < powers.size(); i++)
        powers[i] = powers[i - 1] * K;
}
